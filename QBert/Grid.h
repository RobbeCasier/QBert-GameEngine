#pragma once
#include <BaseComponent.h>
#include <glm\glm.hpp>
#include "Player.h"
#include "CellComponent.h"
#include "Graph.h"
#include "PlayerController.h"

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

	void ConstructPiramid();
	void NewPiramid();
	void CollectDiscs();
	glm::vec2 GetPos(const unsigned short& col, const unsigned short& row) const;
	glm::vec2 GetColumnRow(const unsigned int& index) const;
	int GetIndex(const unsigned int& col, const unsigned int& row) const;
	int GetLiftXDir(const int& col);
	int GetBottomLine() const;

	void CheckGrid(std::shared_ptr<BaseComponent> actor, const int& col, const int& row);
	void CheckCompletion();

	bool NewLevel() const {return m_NewRound;}
	int GetCurrentLevel() { return m_CurrentLevel; }
	int GetCurrentRound() { return m_CurrentRound; }
	Graph* GetGraph() const { return m_pGraph; }
private:
	void ConstructDiscs();

	float m_CellSize = 0.f;
	bool m_NewRound = false;
	int m_CurrentLevel = 1;
	int m_CurrentRound = 1;
	int m_MaxLevels = 3;
	int m_MaxRounds = 4;
	unsigned short m_TopCol = 0;
	unsigned short m_TopRow = 0;
	const unsigned short m_NrCols{10};
	const unsigned short m_NrRows{13};
	const unsigned short m_NrPiramidRows{ 7 };
	std::vector<std::shared_ptr<CellComponent>> m_Cells;
	std::vector<unsigned int> m_DiscCells;
	std::unique_ptr<Subject> m_ActorChanged;

	//commands
	std::vector<std::map<std::string, std::shared_ptr<Command>>> m_Commands;

	Graph* m_pGraph = nullptr;
};

