#include "pch.h"
#include "Grid.h"
#include "Renderer.h"
#include <GameContext.h>
#include <SDL.h>
#include <GameObject.h>
#include "PlayerController.h"

void Grid::Initialize()
{
	//screen is a 4:3 ratio
	SDL_Window* window = GameContext::GetInstance().GetWindow();
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	float cellSize = static_cast<float>(width) / m_NrCols;
	float x = 0, y = 0;
	//loop over all cells in grid
	y = (cellSize / 2.0f);
	for (unsigned int r = 0; r < m_NrRows; r++)
	{
		//if rows are odd shove to the right
		if (r % 2 == 1)
			x = cellSize;
		else
			x = cellSize / 2.0f;
		//set next row center
		for (unsigned int c = 0; c < m_NrCols; c++)
		{
			std::shared_ptr<CellComponent> pCell = m_GameObject->AddComponent<CellComponent>();
			pCell->SetSize(cellSize);
			pCell->SetPosition(x, y);
			m_Cells.push_back(pCell);
			x += cellSize;
		}
		y += (cellSize / 4.0f) * 3.0f;
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

void Grid::CheckGrid(std::shared_ptr<PlayerComponent> actor, const int& col, const int& row)
{
	unsigned int realCol;
	if (row % 2 == 1)
		realCol = (col - 1) / 2;
	else
		realCol = col / 2;

	int index = realCol + (row * m_NrCols);

	if (!m_Cells[index]->IsCube())
	{
		Death deathCommand{ actor };
		deathCommand.Execute();
	}
	else
		m_Cells[index]->ChangeColor();
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
	for (short r = 0; r <= m_NrPiramidRows; r++)
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
		++m_CurrentRound;
		m_CurrentRound = m_CurrentRound % m_MaxRounds;
		if (m_CurrentRound == 1)
		{
			++m_CurrentLevel;
			m_ActorChanged->Notify(shared_from_this(), "NEW_LEVEL");
		}
		else
			m_ActorChanged->Notify(shared_from_this(), "NEW_ROUND");
		m_NewRound = true;
	}
}

void Grid::ConstructPiramid(std::shared_ptr<Player> player, SideColor color, std::vector<int> colorOrder)
{
	//m_TopCol is written in double coordinate system
	//change back to normal
	unsigned short realCol;
	if (m_TopRow % 2 == 1)
		realCol = (m_TopCol - 1) / 2;
	else
		realCol = m_TopCol / 2;

	int index = realCol + (m_TopRow * m_NrCols);

	short row, col = 0;
	for (short r = 0; r <= m_NrPiramidRows; r++)
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
			m_Cells[index]->SetCube(player, shared_from_this(), color, colorOrder, isSide);
		}
	}
}

void Grid::NewPiramid(SideColor color, std::vector<int> colorOrder)
{
	m_NewRound = false;

	unsigned short realCol;
	if (m_TopRow % 2 == 1)
		realCol = (m_TopCol - 1) / 2;
	else
		realCol = m_TopCol / 2;

	int index = realCol + (m_TopRow * m_NrCols);

	short row, col = 0;
	for (short r = 0; r <= m_NrPiramidRows; r++)
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
			block->SetTexture(color);
			block->SetColorOrder(colorOrder);
		}
	}
}
