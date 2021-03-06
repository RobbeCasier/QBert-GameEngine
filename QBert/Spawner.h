#pragma once
#include "Coily.h"
#include "Ugg_Wrongway.h"
#include "Slick_Sam.h"
#include "RedBall.h"
#include "GreenBall.h"
#include <Scene.h>
#include "GameObservers.h"

class Spawner : public BaseComponent, public std::enable_shared_from_this<Spawner>
{
public:
	Spawner();
	virtual ~Spawner() = default;

	//general
	void Initialize();
	virtual void Update();
	void Clear();
	void SetGrid(std::shared_ptr<Grid> pGrid) { m_pGrid = pGrid; }
	void LoadLevelData();

	//coily
	void KillCoily();

	//Ugg & Wrongway
	void SetMaxSpawnUggAndWrongway(unsigned int maxSpawn) { m_MaxSpawnsUggAndWrongway = maxSpawn;}
	void KillUW(std::shared_ptr<BaseComponent> uw);

	//Slick & Sam
	void SetMaxSpawnSlickAndSam(unsigned int maxSpawn) { m_MaxSpawnsSlickAndSam = maxSpawn; }
	void KillSS(std::shared_ptr<BaseComponent> ss);

	//Red Ball
	void KillRB(std::shared_ptr<BaseComponent> rb);

	//Green Ball
	void KillGB();

public:
	//general
	std::shared_ptr<Scene> m_pScene;
	std::shared_ptr<Grid> m_pGrid;
	
	//observers
	std::shared_ptr<Observer> m_obsKillEnemy;

	//coily
	void UpdateCoilySpawn();
	void SpawnCoily();

	//Ugg & Wrongway
	void UpdateUWSpawn();
	void SpawnUW();

	//Slick & Sam
	void UpdateSSSpawn();
	void SpawnSS();

	//Red Ball
	void UpdateRBSpawn();
	void SpawnRB();

	//Green Ball
	void UpdateGBSpawn();
	void SpawnGB();


	//Coily
	std::shared_ptr<GameObject> m_pCoily;
	std::vector<glm::vec2> m_StandardSpawnLocations = { {8,4}, {10,4} };
	int m_MinSpawnTimeCoily = 3;
	int m_MaxSpawnTimeCoily = 12;
	float m_SpawnTimeCoily = 0.f;
	float m_TimerCoily = 0.f;

	//Ugg & Wrongway
	std::vector<glm::vec2> m_UWSpawnLocations = { {15,9}, {3,9} }; //15,9 is the location for ugg the other for wrongway
	std::map<int, std::shared_ptr<GameObject>> m_UggsAndWrongs;
	unsigned int m_MaxSpawnsUggAndWrongway = 0;
	int m_MinSpawnTimeUW = 3;
	int m_MaxSpawnTimeUW = 9;
	float m_SpawnTimeUW = 0.f;
	float m_TimerUW = 0.f;

	//Slick & Sam
	std::map<int, std::shared_ptr<GameObject>> m_SlicksAndSams;
	unsigned int m_MaxSpawnsSlickAndSam = 0;
	int m_MinSpawnTimeSS = 3;
	int m_MaxSpawnTimeSS = 24;
	float m_SpawnTimeSS = 0.f;
	float m_TimerSS = 0.f;

	//Red Ball
	std::map<int, std::shared_ptr<GameObject>> m_RedBalls;
	const float m_SpawnTimeRB = 4.f;
	float m_TimerRB = 0.f;
	bool m_CanSpawnRedBalls = false;

	//Green Ball
	std::shared_ptr<GameObject> m_GreenBall;
	int m_MinSpawnTimeGB = 10;
	int m_MaxSpawnTimeGB = 30;
	float m_SpawnTimeGB = 0.f;
	float m_TimerGB = 0.f;
	bool m_CanSpawnGreenBall = false;
};

