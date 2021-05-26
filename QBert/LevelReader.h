#pragma once
#include <glm\glm.hpp>

class LevelReader
{
public:
	LevelReader() = default;

	void Read(std::string assetFile);
	int GetStyle() const;
	int GetColor() const;
	std::vector<int> GetOrder() const;
	std::vector<glm::vec2> GetDiscs() const;
private:
	void ReadLine(std::ifstream& input);
	bool ReadComment(std::ifstream& input, std::string line);
	bool ReadType(std::ifstream& input, std::string line);
	bool ReadColor(std::ifstream& input, std::string line);
	bool ReadOrder(std::ifstream& input, std::string line);
	bool ReadDisc(std::ifstream& input, std::string line);

	int m_Style = 0;
	int m_Color = 0;
	std::vector<int> m_Order;
	std::vector<glm::vec2> m_Discs{};
};

