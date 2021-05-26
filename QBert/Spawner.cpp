#include "pch.h"
#include "Spawner.h"
#include <GameTime.h>
#include "Ugg_Wrongway.h"
#include "Slick_Sam.h"

Spawner::Spawner(std::shared_ptr<Scene> pScene, std::shared_ptr<Grid> pGrid, std::shared_ptr<Player> pPlayer)
	: m_pScene(pScene)
	, m_pGrid(pGrid)
	, m_pPlayer(pPlayer)
{
}

void Spawner::Initialize()
{
	srand(time(0));
	m_obsKillEnemy = std::make_shared<EnemyDeath>(shared_from_this());
}

void Spawner::Update()
{
	//check if enemies are marked for deletion
	DeleteEnemies();
	UpdateCoilySpawn();
	UpdateUWSpawn();
	UpdateSSSpawn();
}

void Spawner::DeleteEnemies()
{
	if (m_mdCoily)
	{
		m_pScene->Remove(m_pCoily);
		m_pCoily = nullptr;
		m_mdCoily = false;
	}
	if (m_mdUggsAndWrongs.size() != 0)
	{
		for (int id : m_mdUggsAndWrongs)
		{
			auto uw = m_UggsAndWrongs[id];
			m_UggsAndWrongs.erase(m_UggsAndWrongs.begin() + id);
			m_pScene->Remove(uw);
		}
		m_mdUggsAndWrongs.clear();
	}
}

void Spawner::Clear()
{
	if (m_pCoily)
		m_pScene->Remove(m_pCoily);
	for (auto uw : m_UggsAndWrongs)
	{
		m_pScene->Remove(uw);
	}
	m_UggsAndWrongs.clear();

	for (auto ss : m_SlicksAndSams)
	{
		m_pScene->Remove(ss);
	}
	m_SlicksAndSams.clear();
}

void Spawner::UpdateCoilySpawn()
{
	if (!m_pCoily)
	{
		//set new time
		if (m_SpawnTimeCoily < 1.0f)
			m_SpawnTimeCoily = (float) (rand() % (m_MaxSpawnTimeCoily - m_MinSpawnTimeCoily) + m_MinSpawnTimeCoily);
		else
		{
			m_TimerCoily += Time::GetInstance().GetElapsedTime();
			if (m_TimerCoily > m_SpawnTimeCoily)
			{
				SpawnCoily();
				m_TimerCoily = 0.f;
				m_SpawnTimeCoily = 0.f;
			}
		}
	}
}

void Spawner::UpdateUWSpawn()
{
	if (m_UggsAndWrongs.size() < m_MaxSpawnsUggAndWrongway)
	{
		//set new time
		if (m_SpawnTimeUW < 1.0f)
			m_SpawnTimeUW = (float)(rand() % (m_MaxSpawnTimeUW - m_MinSpawnTimeUW) + m_MinSpawnTimeUW);
		else
		{
			m_TimerUW += Time::GetInstance().GetElapsedTime();
			if (m_TimerUW > m_SpawnTimeUW)
			{
				SpawnUW();
				m_TimerUW = 0.f;
				m_SpawnTimeUW = 0.f;
			}
		}
	}
}

void Spawner::UpdateSSSpawn()
{
	if (m_SlicksAndSams.size() < m_MaxSpawnsSlickAndSam)
	{
		//set new time
		if (m_SpawnTimeSS < 1.0f)
			m_SpawnTimeSS = (float)(rand() % (m_MaxSpawnTimeSS - m_MinSpawnTimeSS) + m_MinSpawnTimeSS);
		else
		{
			m_TimerSS += Time::GetInstance().GetElapsedTime();
			if (m_TimerSS > m_SpawnTimeSS)
			{
				SpawnSS();
				m_TimerSS = 0.f;
				m_SpawnTimeSS = 0.f;
			}
		}
	}
}

void Spawner::SpawnCoily()
{
	//choose spawn location
	int randomIdx = rand() % m_StandardSpawnLocations.size();
	auto location = m_StandardSpawnLocations[randomIdx];

	m_pCoily = std::make_shared<GameObject>();
	auto coilyComp = m_pCoily->AddComponent<Coily>();
	coilyComp->SetGrid(m_pGrid);
	coilyComp->AddPlayer(m_pPlayer);
	coilyComp->SetStartLocation(location.x, location.y);
	coilyComp->AddObserver(m_obsKillEnemy);
	m_pScene->Add(m_pCoily);
}


void Spawner::SpawnUW()
{
	//random character
	Type type = (Type)(rand() % (int)Type::count);

	glm::vec2 startLocation = m_UWSpawnLocations[(int)type];

	auto obj = std::make_shared<GameObject>();
	auto uw = std::make_shared<Ugg_Wrongway>(type);
	obj->AddComponent(uw);

	uw->SetGrid(m_pGrid);
	uw->SetPlayer(m_pPlayer);
	uw->SetStartLocation(startLocation.x, startLocation.y);
	uw->AddObserver(m_obsKillEnemy);

	m_UggsAndWrongs.push_back(obj);
	m_pScene->Add(obj);
}


void Spawner::SpawnSS()
{
	//random character
	int randomNr = rand() % m_StandardSpawnLocations.size();

	glm::vec2 startLocation = m_StandardSpawnLocations[randomNr];

	auto obj = std::make_shared<GameObject>();
	auto ss = std::make_shared<Slick_Sam>();
	obj->AddComponent(ss);

	ss->SetGrid(m_pGrid);
	ss->SetPlayer(m_pPlayer);
	ss->SetStartLocation(startLocation.x, startLocation.y);
	ss->AddObserver(m_obsKillEnemy);

	m_SlicksAndSams.push_back(obj);
	m_pScene->Add(obj);
}

void Spawner::KillCoily()
{
	m_mdCoily = true;
}

void Spawner::KillUW(std::shared_ptr<BaseComponent> uw)
{
	int id = std::dynamic_pointer_cast<Ugg_Wrongway>(uw)->GetID();
	m_mdUggsAndWrongs.push_back(id);
}

void Spawner::KillSS(std::shared_ptr<BaseComponent> ss)
{
	int id = std::dynamic_pointer_cast<Slick_Sam>(ss)->GetID();
	m_mdSlicksAndSams.push_back(id);
}
