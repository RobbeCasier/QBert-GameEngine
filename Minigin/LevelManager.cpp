#include "MiniginPCH.h"
#include "LevelManager.h"
#include "Level.h"

void dae::LevelManager::AddLevel(std::shared_ptr<Level> level)
{
	m_Levels.push_back(level);
}

void dae::LevelManager::SetActiveLevel(const std::string& levelName)
{
	for (auto level : m_Levels)
	{
		if (level->GetName()._Equal(levelName))
		{
			m_pActiveLevel = level;
			m_pActiveLevel->Initialize();
		}
		else
			level->Deactivate();
	}
}

void dae::LevelManager::Update()
{
	m_pActiveLevel->Update();
}
