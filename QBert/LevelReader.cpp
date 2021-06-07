#include "pch.h"
#include "LevelReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <rapidjson\document.h>
#include <rapidjson\istreamwrapper.h>
#include <rapidjson\stringbuffer.h>

bool LevelReader::Read(const int& level, const int& round)
{
	m_LevelParameters.order.clear();
	m_LevelParameters.discsPositions.clear();
	std::string file = std::to_string(level) + "_" + std::to_string(round);
	std::string path = "../Data/Levels/" + file;
#ifdef _DEBUG
	path += ".json";
	return ReadJson(path);
#endif
#ifdef NDEBUG
	path += ".bin";
	return ReadBin(path);
#endif

}

bool LevelReader::ReadJson(const std::string& path)
{
	std::ifstream input;
	input.open(path, std::ios::in | std::ios::binary);
	if (!input.is_open())
	{
		std::cout << "Could not open file" << std::endl;
		return false;
	}

	//read whole file
	std::string json, line;
	while (std::getline(input, line))
	{
		json.append(line + "\n");
	}
	input.close();

	rapidjson::Document doc;
	doc.Parse<0>(json.c_str(), json.size());

	for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End(); ++itr)
	{
		const rapidjson::Value& position = *itr;

		m_LevelParameters.gameType = ReadJsonGameType(position, "type");
		m_LevelParameters.sideColor = (SideColor)ReadJsonInteger(position, "color");
		m_LevelParameters.order = ReadJsonOrder(position, "order");
		m_LevelParameters.discsPositions = ReadJsonDiscLocation(position, "disc");
		m_LevelParameters.spawnRedBall = ReadJsonBool(position, "red");
		m_LevelParameters.spawnGreenBall = ReadJsonBool(position, "green");
		m_LevelParameters.maxUWSpawn = ReadJsonInteger(position, "uw");
		m_LevelParameters.maxSSSpawn = ReadJsonInteger(position, "ss");
		m_LevelParameters.bonus = ReadJsonInteger(position, "bonus");
	}
	return true;

}

GameType LevelReader::ReadJsonGameType(const rapidjson::Value& position, const std::string& typeName)
{
	const rapidjson::Value& value = position[typeName.c_str()];

	std::string str = value.GetString();
	if (str._Equal("single"))
		return GameType::singleColor;
	else if (str._Equal("double"))
		return GameType::doubleColor;
	else if (str._Equal("single_repeat"))
		return GameType::RepeatSingleColor;
	return GameType{};
}

std::vector<int> LevelReader::ReadJsonOrder(const rapidjson::Value& position, const std::string& typeName)
{
	const rapidjson::Value& value = position[typeName.c_str()];

	std::vector<int> order;
	for (rapidjson::Value::ConstValueIterator itr = value.Begin(); itr < value.End(); ++itr)
	{
		const rapidjson::Value& positionArray = *itr;

		order.push_back(itr->GetInt());
	}
	return order;
}

std::vector<glm::vec2> LevelReader::ReadJsonDiscLocation(const rapidjson::Value& position, const std::string& typeName)
{
	const rapidjson::Value& value = position[typeName.c_str()];

	std::vector<glm::vec2> positions;
	for (rapidjson::Value::ConstValueIterator itr = value.Begin(); itr < value.End(); ++itr)
	{
		const rapidjson::Value& positionArray = *itr;
		int column , row;

		//get column
		column = ReadJsonInteger(positionArray, "c");

		//get row
		row = ReadJsonInteger(positionArray, "r");
		positions.push_back({ column, row });
	}
	return positions;
}

int LevelReader::ReadJsonInteger(const rapidjson::Value& position, const std::string& typeName)
{
	const rapidjson::Value& value = position[typeName.c_str()];

	int integer = value.GetInt();
	return integer;
}

bool LevelReader::ReadJsonBool(const rapidjson::Value& position, const std::string& typeName)
{
	const rapidjson::Value& value = position[typeName.c_str()];

	bool boolean = value.GetBool();
	return boolean;
}

bool LevelReader::ReadBin(const std::string& path)
{
	std::ifstream input;
	input.open(path, std::ios::in | std::ios::binary);
	if (!input.is_open())
	{
		std::cout << "Could not open file" << std::endl;
		return false;
	}

	//read gametype
	char gameTypeLength = 'a';
	int size = 0;
	std::string type;
	input.read((char*)&gameTypeLength, sizeof(char));
	size = (int)gameTypeLength;
	input.read(&type[0], size);
	if (type._Equal("single"))
		m_LevelParameters.gameType = GameType::singleColor;
	else if (type._Equal("double"))
		m_LevelParameters.gameType = GameType::doubleColor;
	else if (type._Equal("single_repeat"))
		m_LevelParameters.gameType = GameType::RepeatSingleColor;

	//side color
	input.read((char*)&m_LevelParameters.sideColor, sizeof(int));

	//order
	char amount = 'a';
	input.read((char*)&amount, sizeof(char));
	int orders = (int)amount;
	for (int i = 0; i < orders; ++i)
	{
		int nr = 0;
		input.read((char*)&nr, sizeof(int));
		m_LevelParameters.order.push_back(nr);
	}

	//discs positions
	input.read((char*)&size, sizeof(int));
	size /= 2 * sizeof(int);
	for (int i = 0; i < size; ++i)
	{
		int x = 0, y = 0;
		input.read((char*)&x, sizeof(int));
		input.read((char*)&y, sizeof(int));
		m_LevelParameters.discsPositions.push_back({ x,y });
	}

	//spawn red ball
	input.read((char*)&m_LevelParameters.spawnRedBall, sizeof(bool));
	//spawn green ball
	input.read((char*)&m_LevelParameters.spawnGreenBall, sizeof(bool));

	//max uw spawn
	input.read((char*)&m_LevelParameters.maxUWSpawn, sizeof(int));

	//max ss spawn
	input.read((char*)&m_LevelParameters.maxSSSpawn, sizeof(int));

	//bonus
	input.read((char*)&m_LevelParameters.bonus, sizeof(int));
	return true;
}
