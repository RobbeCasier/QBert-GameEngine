#include "pch.h"
#include "LevelReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

void LevelReader::Read(std::string assetFile)
{
	m_Order.clear();
	std::ifstream input;
	input.open(assetFile, std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (!input.eof())
			ReadLine(input);
		
	}
}

int LevelReader::GetStyle() const
{
	return m_Style;
}

int LevelReader::GetColor() const
{
	return m_Color;
}

std::vector<int> LevelReader::GetOrder() const
{
	return m_Order;
}

void LevelReader::ReadLine(std::ifstream& input)
{
	std::string line;
	std::getline(input, line, '\n');

	if (line != "")
		if (!ReadComment(input, line))
			if (!ReadType(input, line))
				if (!ReadColor(input, line))
					ReadOrder(input, line);

}

bool LevelReader::ReadComment(std::ifstream& input, std::string line)
{
	std::string tempString = line;
	std::regex regex = std::regex{ "#.*" };
	if (std::regex_match(tempString, regex))
		return true;
	return false;
}

bool LevelReader::ReadType(std::ifstream& input, std::string line)
{
	std::string tempString = line;
	std::regex regex = std::regex{ "(?:<TYPE>)" };
	if (std::regex_search(tempString, regex))
	{
		tempString = tempString.substr(7, 1);
		m_Style = std::stoi(tempString);
		return true;
	}
	return false;
}

bool LevelReader::ReadColor(std::ifstream& input, std::string line)
{
	std::string tempString = line;
	std::regex regex = std::regex{ "(?:<COLOR>)" };
	if (std::regex_search(tempString, regex))
	{
		tempString = tempString.substr(8, 1);
		m_Color = std::stoi(tempString);
		return true;
	}
	return false;
}

bool LevelReader::ReadOrder(std::ifstream& input, std::string line)
{
	std::string tempString = line;
	std::regex regex = std::regex{ "(?:<ORDER>)" };
	int offset = 8;
	if (std::regex_search(tempString, regex))
	{
		while (offset < tempString.size())
		{
			std::string sub;
			sub = tempString.substr(offset, 1);
			offset += 2;
			m_Order.push_back(std::stoi(sub));
		}
		return true;
	}
	return false;
}