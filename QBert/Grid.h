#pragma once
#include <BaseComponent.h>
#include <glm\glm.hpp>
#include "Player.h"
#include "CellComponent.h"

//******************//
//Double Coordinates//
//******************//
class Grid : public BaseComponent, public std::enable_shared_from_this<Grid>
{
public:
	explicit Grid() = default;
	virtual ~Grid() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	void AddObserver(std::shared_ptr<Observer> observer);

	void SetTopPiramid(const unsigned short& col, const unsigned short& row);

	void ConstructPiramid(std::shared_ptr<Player> player, SideColor color, std::vector<int> colorOrder);
	void NewPiramid(SideColor color, std::vector<int> colorOrder);
	glm::vec2 GetPos(const unsigned short& col, const unsigned short& row) const;

	void CheckGrid(std::shared_ptr<PlayerComponent> actor, const int& col, const int& row);
	void CheckCompletion();

	bool NewLevel() const {return m_NewRound;}
	int GetCurrentLevel() { return m_CurrentLevel; }
	int GetCurrentRound() { return m_CurrentRound; }
private:

	bool m_NewRound;
	int m_CurrentLevel = 1;
	int m_CurrentRound = 1;
	int m_MaxLevels = 3;
	int m_MaxRounds = 4;
	unsigned short m_TopCol;
	unsigned short m_TopRow;
	const unsigned short m_NrCols{10};
	const unsigned short m_NrRows{12};
	const unsigned short m_NrPiramidRows{ 7 };
	std::vector<std::shared_ptr<CellComponent>> m_Cells;
	std::unique_ptr<Subject> m_ActorChanged;
};

