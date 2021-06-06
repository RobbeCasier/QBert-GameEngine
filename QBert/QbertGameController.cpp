#include "pch.h"
#include "QbertGameController.h"
#include <SceneManager.h>
#include <LevelManager.h>
#include "MainMenuLevel.h"

void QbertGameController::Initialize()
{
		m_pMainMenu = std::make_shared<MainMenuLevel>();
		dae::LevelManager::GetInstance().AddLevel(m_pMainMenu);
		m_pGame = std::make_shared<MainLevel>();
		dae::LevelManager::GetInstance().AddLevel(m_pGame);
}

void QbertGameController::Update()
{
	if (!m_StateUpdated)
		return;

	m_StateUpdated = false;
	switch (m_State)
	{
	case QbertGameState::MAINMENU:
		dae::LevelManager::GetInstance().SetActiveLevel("MainMenuLevel");
		break;
	case QbertGameState::LEVEL:
		dae::LevelManager::GetInstance().SetActiveLevel("Game");
		dae::SceneManager::GetInstance().UnloadScene("InGameMenu");
		break;
	case QbertGameState::PAUSE:
		dae::SceneManager::GetInstance().LoadScene("InGameMenu");
		break;
	case QbertGameState::END:
		break;
	default:
		break;
	}
}
