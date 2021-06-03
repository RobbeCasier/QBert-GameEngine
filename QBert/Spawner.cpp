#include "pch.h"
#include "Spawner.h"
#include <GameTime.h>
#include "Ugg_Wrongway.h"
#include "Slick_Sam.h"
#include "RedBall.h"
#include "GreenBall.h"

Spawner::Spawner(std::shared_ptr<Scene> pScene, std::shared_ptr<Grid> pGrid, const std::vector<std::shared_ptr<Player>>& players)
	: m_pScene(pScene)
	, m_pGrid(pGrid)
	, m_Players(players)
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
	UpdateRBSpawn();
}

void Spawner::DeleteEnemies()
{
	if (m_mdCoily)
	{
		m_pScene->RemoveObject(m_pCoily);
		m_pCoily = nullptr;
		m_mdCoily = false;
	}
	if (m_mdUggsAndWrongs.size() != 0)
	{
		for (int id : m_mdUggsAndWrongs)
		{
			auto it = m_UggsAndWrongs.find(id);
			if (it == m_UggsAndWrongs.end())
				continue;
			m_pScene->RemoveObject(it->second);
			m_UggsAndWrongs.erase(it);
		}
		m_mdUggsAndWrongs.clear();
	}
	if (m_mdSlicksAndSams.size() != 0)
	{
		for (int id : m_mdSlicksAndSams)
		{
			auto it = m_SlicksAndSams.find(id);
			if (it == m_SlicksAndSams.end())
				continue;
			m_pScene->RemoveObject(it->second);
			m_SlicksAndSams.erase(it);
		}
		m_mdSlicksAndSams.clear();
	}
	if (m_mdRedBalls.size() != 0)
	{
		for (int id : m_mdRedBalls)
		{
			auto it = m_RedBalls.find(id);
			if (it == m_RedBalls.end())
				continue;
			m_pScene->RemoveObject(it->second);
			m_RedBalls.erase(it);
		}
		m_mdRedBalls.clear();
	}
}

void Spawner::Clear()
{
	if (m_pCoily)
	{
		m_pScene->RemoveObject(m_pCoily);
		m_pCoily = nullptr;
	}
	m_TimerCoily = 0.f;
	m_SpawnTimeCoily = 0.f;

	for (auto uw : m_UggsAndWrongs)
	{
		m_pScene->RemoveObject(uw.second);
	}
	m_UggsAndWrongs.clear();
	m_SpawnTimeUW = 0.f;
	m_TimerUW = 0.f;

	for (auto ss : m_SlicksAndSams)
	{
		m_pScene->RemoveObject(ss.second);
	}
	m_SlicksAndSams.clear();
	m_SpawnTimeSS = 0.f;
	m_TimerSS = 0.f;

	for (auto rb : m_RedBalls)
	{
		m_pScene->RemoveObject(rb.second);
	}
	m_RedBalls.clear();
	m_TimerRB = 0.f;
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

void Spawner::UpdateRBSpawn()
{
	m_TimerRB += Time::GetInstance().GetElapsedTime();
	if (m_TimerRB > m_SpawnTimeRB)
	{
		SpawnRB();
		m_TimerRB = 0.f;
	}
}

void Spawner::UpdateGBSpawn()
{
	if (!m_GreenBall)
	{
		//set new time
		if (m_SpawnTimeGB < 1.0f)
			m_SpawnTimeGB = (float)(rand() % (m_MaxSpawnTimeCoily - m_MinSpawnTimeCoily) + m_MinSpawnTimeCoily);
		else
		{
			m_TimerGB += Time::GetInstance().GetElapsedTime();
			if (m_TimerGB > m_SpawnTimeGB)
			{
				SpawnGB();
				m_TimerGB = 0.f;
				m_SpawnTimeGB = 0.f;
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
	coilyComp->AddPlayers(m_Players);
	coilyComp->SetStartLocation((int)location.x, (int)location.y);
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
	uw->SetPlayers(m_Players);
	uw->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	uw->AddObserver(m_obsKillEnemy);

	m_UggsAndWrongs.insert(std::pair<int, std::shared_ptr<GameObject>>(uw->GetID(),obj));
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
	ss->SetPlayers(m_Players);
	ss->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	ss->AddObserver(m_obsKillEnemy);

	m_SlicksAndSams.insert(std::pair<int, std::shared_ptr<GameObject>>(ss->GetID(), obj));
	m_pScene->Add(obj);
}


void Spawner::SpawnRB()
{
	//random spawn location
	int randomNr = rand() % m_StandardSpawnLocations.size();
	glm::vec2 startLocation = m_StandardSpawnLocations[randomNr];

	auto obj = std::make_shared<GameObject>();
	auto rb = std::make_shared<RedBall>();
	obj->AddComponent(rb);

	rb->SetGrid(m_pGrid);
	rb->SetPlayers(m_Players);
	rb->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	rb->AddObserver(m_obsKillEnemy);

	std::cout << rb->GetID() << std::endl;
	m_RedBalls.insert(std::pair<int, std::shared_ptr<GameObject>>(rb->GetID(), obj));
	m_pScene->Add(obj);
}

void Spawner::SpawnGB()
{
	//choose spawn location
	int randomIdx = rand() % m_StandardSpawnLocations.size();
	auto location = m_StandardSpawnLocations[randomIdx];

	m_GreenBall = std::make_shared<GameObject>();
	auto gb = m_GreenBall->AddComponent<GreenBall>();
	gb->SetGrid(m_pGrid);
	gb->SetPlayers(m_Players);
	gb->SetStartLocation((int)location.x, (int)location.y);
	gb->AddObserver(m_obsKillEnemy);
	m_pScene->Add(m_GreenBall);
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

void Spawner::KillRB(std::shared_ptr<BaseComponent> rb)
{
	auto id = std::dynamic_pointer_cast<RedBall>(rb)->GetID();
	m_mdRedBalls.push_back(id);
}

void Spawner::KillGB()
{
	m_pScene->RemoveObject(m_GreenBall);
	m_GreenBall = nullptr;
}
