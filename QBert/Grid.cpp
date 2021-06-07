#include "pch.h"
#include "Grid.h"
#include "Renderer.h"
#include <GameContext.h>
#include <SDL.h>
#include <GameObject.h>
#include "Coily.h"
#include "Ugg_Wrongway.h"
#include "Slick_Sam.h"
#include "RedBall.h"
#include "GreenBall.h"
#include "EnemyController.h"
#include "LevelReader.h"
#include "PlayerManager.h"

void Grid::Initialize()
{
	//screen is a 4:3 ratio
	SDL_Window* window = GameContext::GetInstance().GetWindow();
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	m_CellSize = static_cast<float>(width) / m_NrCols;
	float x = 0, y = 0;
	//loop over all cells in grid
	//y = (m_CellSize / 2.0f);
	for (unsigned int r = 0; r < m_NrRows; r++)
	{
		//if rows are odd shove to the right
		if (r % 2 == 1)
			x = m_CellSize;
		else
			x = m_CellSize / 2.0f;
		//set next row center
		for (unsigned int c = 0; c < m_NrCols; c++)
		{
			std::shared_ptr<CellComponent> pCell = m_GameObject->AddComponent<CellComponent>();
			pCell->SetSize(m_CellSize);
			pCell->SetPosition(x, y);
			m_Cells.push_back(pCell);
			x += m_CellSize;
		}
		y += (m_CellSize / 4.0f) * 3.0f;
	}

}

void Grid::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_ActorChanged == nullptr)
		m_ActorChanged = std::make_unique<Subject>();
	m_ActorChanged->AddObserver(observer);
}

//both column and row have to be of the same kind
//if row is odd, column will also be odd
void Grid::SetTopPiramid(const unsigned short& col, const unsigned short& row)
{
	m_TopRow = row;
	if (row % 2 != col % 2)
		m_TopCol = col + 1;
	else
		m_TopCol = col;
}

//position for sprite double coordinate system
glm::vec2 Grid::GetPos(const unsigned short& col, const unsigned short& row) const
{
	unsigned short realCol;
	if (row % 2 == 1)
		realCol = (col - 1) / 2;
	else
		realCol = col / 2;

	int index = realCol + (row * m_NrCols);

	return m_Cells[index]->GetSpritePos();
}

glm::vec2 Grid::GetTop() const
{
	return glm::vec2(m_TopCol, m_TopRow);
}

glm::vec2 Grid::GetColumnRow(const unsigned int& index) const
{
	int row = index / m_NrCols;
	int col = index % m_NrCols;

	if (row % 2 == 1)
		col = col * 2 + 1;
	else
		col = col * 2;
	return glm::vec2(col, row);
}

int Grid::GetIndex(const unsigned int& col, const unsigned int& row) const
{
	unsigned int realCol;
	if (row % 2 == 1)
		realCol = (col - 1) / 2;
	else
		realCol = col / 2;

	int index = realCol + (row * m_NrCols);
	return index;
}

int Grid::GetLiftXDir(const int& col)
{
	if (col < m_TopCol)
		return 1;
	return -1;
}

int Grid::GetBottomLine() const
{
	return m_TopRow + m_NrPiramidRows - 1;
}

void Grid::CheckGrid(std::shared_ptr<BaseComponent> actor, const int& col, const int& row)
{
	unsigned int realCol;
	if (row % 2 == 1)
		realCol = (col - 1) / 2;
	else
		realCol = col / 2;

	int index = realCol + (row * m_NrCols);

	std::shared_ptr<Coily> coily = std::dynamic_pointer_cast<Coily>(actor);
	std::shared_ptr<Ugg_Wrongway> uw = std::dynamic_pointer_cast<Ugg_Wrongway>(actor);
	std::shared_ptr<Slick_Sam> ss = std::dynamic_pointer_cast<Slick_Sam>(actor);
	std::shared_ptr<RedBall> rb = std::dynamic_pointer_cast<RedBall>(actor);
	std::shared_ptr<GreenBall> gb = std::dynamic_pointer_cast<GreenBall>(actor);

	//find player
	auto players = PlayerManager::GetInstance().GetPlayers();
	auto it = std::find(players.begin(), players.end(), actor);
	int cmdIndex = -1;
	//if player exist get corresponding command index
	if (it != players.end())
	{
		cmdIndex = (int)std::distance(players.begin(), it);
	}
	
	//is the current landed cell empty
	if (!m_Cells[index]->IsCube() && !m_Cells[index]->IsDisc())
	{
		//if coily add points
		if (coily)
		{
			const int& player = coily->GetPlayer();
			m_Commands[player].at("BeatCoily")->Execute();
			m_ActorChanged->Notify(actor, "FALL_COILY");
		}
		else if (uw)
		{
			m_ActorChanged->Notify(actor, "FALL_UW");
		}
		else if (ss)
		{
			m_ActorChanged->Notify(actor, "FALL_SS");
		}
		else if (rb)
		{
			m_ActorChanged->Notify(actor, "FALL_RB");
		}
		else if (gb)
		{
			m_ActorChanged->Notify(actor, "FALL_GB");
		}
		else
		{
			if (cmdIndex >= 0)
			{
				m_Commands[cmdIndex].at("Fall")->Execute();
			}
		}
	}
	//is the cell a cube
	else if (m_Cells[index]->IsCube())
	{
		ColorState colorState;
		if (coily || uw || rb || gb)
			return;
		else if (ss)
			colorState = m_Cells[index]->ChangeColor(true);
		else
			colorState = m_Cells[index]->ChangeColor();

		//does player exist
		if (index >= 0)
		{
			switch (colorState)
			{
			case ColorState::INTERMEDIAT:
				m_Commands[cmdIndex].at("Intermediat")->Execute();
				break;
			case ColorState::FINAL:
				m_Commands[cmdIndex].at("Final")->Execute();
				CheckCompletion();
				break;
			default:
				break;
			}
		}
	}
	//is the cell a disc
	else
	{
		if (coily || uw)
			return;

		//does player exist
		if (cmdIndex >= 0)
		{
			m_Commands[cmdIndex].at("Lift")->Execute();
		}
		m_Cells[index]->RemoveDisc();
	}

}

void Grid::CheckCompletion()
{
	unsigned short realCol;
	if (m_TopRow % 2 == 1)
		realCol = (m_TopCol - 1) / 2;
	else
		realCol = m_TopCol / 2;

	int index = realCol + (m_TopRow * m_NrCols);

	short row, col = 0;
	bool completed = false;
	for (short r = 0; r < m_NrPiramidRows; r++)
	{
		row = m_TopRow + r;
		if (r % 2 == 0)
			col = realCol - r / 2;

		for (short c = 0; c < r + 1; c++)
		{
			bool isSide = false;
			if (c == 0 || c == r)
				isSide = true;
			index = (col + c) + (row * m_NrCols);
			completed = m_Cells[index]->GetBlock()->IsRightColor();
			if (!completed)
				break;
		}
		if (!completed)
			break;
	}
	if (completed)
	{
		//give all players bonus
		for (const auto& commands : m_Commands)
		{
			commands.at("Bonus")->Execute();
		}

		++m_CurrentRound;
		//set max to 5
		m_CurrentRound = m_CurrentRound % (m_MaxRounds+1);
		//from 4 to 5 to 0
		if (m_CurrentRound == 0)
		{
			//add level
			++m_CurrentLevel;
			//back to 1
			++m_CurrentRound;
			m_ActorChanged->Notify(shared_from_this(), "NEW_LEVEL");
			m_ActorChanged->Notify(shared_from_this(), "NEW_ROUND");
		}
		else
			m_ActorChanged->Notify(shared_from_this(), "NEW_ROUND");
		m_NewRound = true;
	}
}

void Grid::ConstructPiramid()
{
	//setup player
	auto players = PlayerManager::GetInstance().GetPlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		std::map<std::string, std::shared_ptr<Command>> commands;
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("BeatCoily", std::make_unique<BeatCoily>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("RemainingDisc", std::make_unique<RemainingDisc>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("Bonus", std::make_unique<Bonus>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("Fall", std::make_unique<Fall>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("Lift", std::make_unique<Lift>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("Intermediat", std::make_unique<ChangeColorPlayerIntermediat>(players[i])));
		commands.insert(std::pair<std::string, std::shared_ptr<Command>>("Final", std::make_unique<ChangeColorPlayerFinal>(players[i])));
		m_Commands.push_back(commands);
	}
	//m_TopCol is written in double coordinate system
	//change back to normal
	unsigned short realCol;
	if (m_TopRow % 2 == 1)
		realCol = (m_TopCol - 1) / 2;
	else
		realCol = m_TopCol / 2;

	int index = realCol + (m_TopRow * m_NrCols);

	const LevelParameters& lvParam = LevelReader::GetInstance().GetLevelParamters();

	short row, col = 0;
	for (short r = 0; r < m_NrPiramidRows; r++)
	{
		row = m_TopRow + r;
		if (r%2 ==0)
			col = realCol - r/2;
			
		for (short c = 0; c < r+1; c++)
		{
			bool isSide = false;
			if (c == 0 || c == r)
				isSide = true;
			index = (col + c) + (row * m_NrCols);
			m_Cells[index]->SetCube(lvParam.sideColor, lvParam.order, isSide);
		}
	}
	ConstructDiscs();

	m_pGraph = new Graph();
	m_pGraph->CreateGraphFromGrid(m_NrCols, m_NrRows, m_Cells);
}

void Grid::ConstructDiscs()
{
	const LevelParameters& lvParam = LevelReader::GetInstance().GetLevelParamters();
	const std::vector<glm::vec2>& pos = lvParam.discsPositions;
	int x, y;
	for (int i = 0; i < pos.size(); ++i)
	{
		x = (int)pos[i].x;
		y = (int)pos[i].y;
		if (y % 2 == 1)
			x = (x - 1) / 2;
		else
			x = x / 2;

		int index = x + (y * m_NrCols);
		m_DiscCells.push_back(index);

		m_Cells[index]->SetDisc(lvParam.sideColor);
	}
}

void Grid::NewPiramid()
{
	auto players = PlayerManager::GetInstance().GetPlayers();
	for (auto& player : players)
		player->Reset();

	m_NewRound = false;

	unsigned short realCol;
	if (m_TopRow % 2 == 1)
		realCol = (m_TopCol - 1) / 2;
	else
		realCol = m_TopCol / 2;

	int index = realCol + (m_TopRow * m_NrCols);

	const LevelParameters& lvParam = LevelReader::GetInstance().GetLevelParamters();

	short row, col = 0;
	for (short r = 0; r < m_NrPiramidRows; r++)
	{
		row = m_TopRow + r;
		if (r % 2 == 0)
			col = realCol - r / 2;

		for (short c = 0; c < r + 1; c++)
		{
			bool isSide = false;
			if (c == 0 || c == r)
				isSide = true;
			index = (col + c) + (row * m_NrCols);
			auto block = m_Cells[index]->GetBlock();
			block->SetTexture(lvParam.sideColor);
			block->SetColorOrder(lvParam.order);
			block->SetGameType(lvParam.gameType);
		}
	}

	ConstructDiscs();
}

void Grid::CollectDiscs()
{
	//check if there are any left from previous level
	//and destroy
	if (m_DiscCells.size() > 0)
	{
		for (auto index : m_DiscCells)
		{
			if (m_Cells[index]->IsDisc())
			{
				for (const auto& commands : m_Commands)
				{
					commands.at("RemainingDisc")->Execute();
				}
				m_Cells[index]->RemoveDisc();
			}
			
		}
		m_DiscCells.clear();
	}
}
