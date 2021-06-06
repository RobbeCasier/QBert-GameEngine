#include "pch.h"
#include "Spawner.h"
#include <GameTime.h>
#include <ServiceLocator.h>
#include "QbertGameController.h"

Spawner::Spawner()
{
}

void Spawner::Initialize()
{
	srand(time(0));
	m_obsKillEnemy = std::make_shared<EnemyDeath>(shared_from_this());
}

void Spawner::Update()
{
	if (!m_pScene)
		m_pScene = GetGameObject()->GetScene();

	if (GameContext::GetInstance().GetGameState() != GameState::PLAY)
		return;

	UpdateCoilySpawn();
	UpdateUWSpawn();
	UpdateSSSpawn();
	if (m_CanSpawnRedBalls)
		UpdateRBSpawn();
	if (m_CanSpawnGreenBall)
		UpdateGBSpawn();
}

void Spawner::Clear()
{
	auto pScene = GetGameObject()->GetScene();
	if (m_pCoily)
	{
		pScene->RemoveObject(m_pCoily);
		m_pCoily = nullptr;
	}
	m_TimerCoily = 0.f;
	m_SpawnTimeCoily = 0.f;

	for (auto uw : m_UggsAndWrongs)
	{
		pScene->RemoveObject(uw.second);
	}
	m_UggsAndWrongs.clear();
	m_SpawnTimeUW = 0.f;
	m_TimerUW = 0.f;

	for (auto ss : m_SlicksAndSams)
	{
		pScene->RemoveObject(ss.second);
	}
	m_SlicksAndSams.clear();
	m_SpawnTimeSS = 0.f;
	m_TimerSS = 0.f;

	for (auto rb : m_RedBalls)
	{
		pScene->RemoveObject(rb.second);
	}
	m_RedBalls.clear();
	m_TimerRB = 0.f;
	if (m_GreenBall)
	{
		pScene->RemoveObject(m_GreenBall);
		m_GreenBall = nullptr;
	}
}

void Spawner::LoadLevelData()
{
	const LevelParameters& lvParams = LevelReader::GetInstance().GetLevelParamters();
	m_MaxSpawnsUggAndWrongway = lvParams.maxUWSpawn;
	m_MaxSpawnsSlickAndSam = lvParams.maxSSSpawn;
	m_CanSpawnRedBalls = lvParams.spawnRedBall;
	m_CanSpawnGreenBall = lvParams.spawnGreenBall;
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
	auto pScene = GetGameObject()->GetScene();
	//choose spawn location
	int randomIdx = rand() % m_StandardSpawnLocations.size();
	auto location = m_StandardSpawnLocations[randomIdx];

	m_pCoily = std::make_shared<GameObject>();
	auto coilyComp = m_pCoily->AddComponent<Coily>();
	coilyComp->SetGrid(m_pGrid);
	coilyComp->SetStartLocation((int)location.x, (int)location.y);
	coilyComp->AddObserver(m_obsKillEnemy);
	if (((QbertGameController&)ServiceLocator::GetGameController()).GetQbertGameMode() == GameMode::VS)
		coilyComp->SetPossesed();
	m_pScene->AddObject(m_pCoily);
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
	uw->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	uw->AddObserver(m_obsKillEnemy);

	m_UggsAndWrongs.insert(std::pair<int, std::shared_ptr<GameObject>>(uw->GetID(),obj));
	m_pScene->AddObject(obj);
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
	ss->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	ss->AddObserver(m_obsKillEnemy);

	m_SlicksAndSams.insert(std::pair<int, std::shared_ptr<GameObject>>(ss->GetID(), obj));
	m_pScene->AddObject(obj);
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
	rb->SetStartLocation((int)startLocation.x, (int)startLocation.y);
	rb->AddObserver(m_obsKillEnemy);

	m_RedBalls.insert(std::pair<int, std::shared_ptr<GameObject>>(rb->GetID(), obj));
	m_pScene->AddObject(obj);
}

void Spawner::SpawnGB()
{
	//choose spawn location
	int randomIdx = rand() % m_StandardSpawnLocations.size();
	auto location = m_StandardSpawnLocations[randomIdx];

	m_GreenBall = std::make_shared<GameObject>();
	auto gb = m_GreenBall->AddComponent<GreenBall>();
	gb->SetGrid(m_pGrid);
	gb->SetStartLocation((int)location.x, (int)location.y);
	gb->AddObserver(m_obsKillEnemy);
	m_pScene->AddObject(m_GreenBall);
}

void Spawner::KillCoily()
{
	m_pScene->RemoveObject(m_pCoily);
	m_pCoily = nullptr;
}

void Spawner::KillUW(std::shared_ptr<BaseComponent> uw)
{
	int id = std::dynamic_pointer_cast<Ugg_Wrongway>(uw)->GetID();
	auto it = m_UggsAndWrongs.find(id);
	if (it == m_UggsAndWrongs.end())
		return;
	m_pScene->RemoveObject(it->second);
	m_UggsAndWrongs.erase(it);
}

void Spawner::KillSS(std::shared_ptr<BaseComponent> ss)
{
	int id = std::dynamic_pointer_cast<Slick_Sam>(ss)->GetID();
	auto it = m_SlicksAndSams.find(id);
	if (it == m_SlicksAndSams.end())
		return;
	m_pScene->RemoveObject(it->second);
	m_SlicksAndSams.erase(it);
}

void Spawner::KillRB(std::shared_ptr<BaseComponent> rb)
{
	auto id = std::dynamic_pointer_cast<RedBall>(rb)->GetID();
	auto it = m_RedBalls.find(id);
	if (it == m_RedBalls.end())
		return;
	m_pScene->RemoveObject(it->second);
	m_RedBalls.erase(it);
}

void Spawner::KillGB()
{
	m_pScene->RemoveObject(m_GreenBall);
	m_GreenBall = nullptr;
}
