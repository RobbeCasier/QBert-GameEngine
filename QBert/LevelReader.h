#pragma once
#include <glm\glm.hpp>
#include <Singleton.h>

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>

enum class SideColor
{
	aqua,
	orange,
	grey,
	greyBlue,
	black,
	yellowOrange,
	yellowBlue,
	green,
	blackBlack
};
enum class GameType
{
	singleColor,
	doubleColor,
	RepeatSingleColor
};

struct LevelParameters
{
	std::vector<int> order;
	std::vector<glm::vec2> discsPositions;
	int maxUWSpawn;
	int maxSSSpawn;
	int bonus;
	GameType gameType;
	SideColor sideColor;
	bool spawnRedBall;
	bool spawnGreenBall;
};
class LevelReader final : public dae::Singleton<LevelReader>
{
public:
	LevelReader() = default;

	void Read(const int& level, const int& round);
	LevelParameters GetLevelParamters() { return m_LevelParameters; }
private:
	void ReadJson(const std::string& path);
	GameType ReadJsonGameType(const rapidjson::Value& position, const std::string& typeName);
	std::vector<int> ReadJsonOrder(const rapidjson::Value& position, const std::string& typeName);
	std::vector<glm::vec2> ReadJsonDiscLocation(const rapidjson::Value& position, const std::string& typeName);
	int ReadJsonInteger(const rapidjson::Value& position, const std::string& typeName);
	bool ReadJsonBool(const rapidjson::Value& position, const std::string& typeName);
	void ReadBin(const std::string& path);

	LevelParameters m_LevelParameters;
};

