#include "pch.h"
#include "LevelReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <rapidjson\document.h>
#include <rapidjson\istreamwrapper.h>
#include <rapidjson\stringbuffer.h>

#define psln(x) std::cout<<#x " = "<<(x)<<std::endl

void LevelReader::Read(const int& level, const int& round)
{
	m_LevelParameters.order.clear();
	m_LevelParameters.discsPositions.clear();
	std::string file = std::to_string(level) + "_" + std::to_string(round);
#ifdef _DEBUG
	std::string path = "../Data/Levels/" + file + ".json";
	ReadJson(path);
#endif
#ifdef _RELEASE
#endif

}

void LevelReader::ReadJson(const std::string& path)
{
	std::ifstream input;
	input.open(path, std::ios::in | std::ios::binary);
	if (!input.is_open())
	{
		std::cout << "Could not open file" << std::endl;
		return;
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
	if (doc.HasParseError())
	{
		rapidjson::ParseErrorCode code = doc.GetParseError();
		psln(code);
	}

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
	input.close();

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

void LevelReader::ReadBin(const std::string& path)
{
}
