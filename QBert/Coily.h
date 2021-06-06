#pragma once
#include "Player.h"
#include "Graph.h"
#include "CellComponent.h"
#include "PlayerController.h"

class Grid;

enum class CoilyState
{
	egg,
	snake
};

class Coily : public Player
{
public:
	explicit Coily() : Player{} {};
	virtual ~Coily() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	const int& GetPlayer();

	void SetPossesed() { m_IsPossesed = true; }
	bool IsPossesed() { return m_IsPossesed; }

private:
	virtual void StopFall();
	virtual void StopDescend();
	virtual void ChangeLookDirection(const int& horDir = 0, const int& verDir = 0);
	void GetRandomJumpLocation();

	void CheckCollision();

	const unsigned int m_NrColsEgg{ 2 };
	const unsigned int m_NrRowsEgg{ 1 };

	const float m_TimeTilNextJump = 0.3f;
	const float m_TimeTillHatch = 3.0f;
	float m_Timer = 0.f;

	CoilyState m_CState = CoilyState::egg;

	Graph* m_pGraph = nullptr;

	std::vector<std::shared_ptr<Command>> m_cmdPlayerDeaths;

	bool m_IsPossesed = false;
};

