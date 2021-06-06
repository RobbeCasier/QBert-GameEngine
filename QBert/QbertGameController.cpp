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
		dae::SceneManager::GetInstance().LoadScene("MainMenu");
		break;
	case QbertGameState::LEVEL:
		if (m_PreviousState != QbertGameState::PAUSE)
			dae::LevelManager::GetInstance().SetActiveLevel("Game");

		dae::SceneManager::GetInstance().LoadScene("GameScene");
		dae::SceneManager::GetInstance().LoadScene("HudScene");
		dae::SceneManager::GetInstance().UnloadScene("InGameMenu");
		break;
	case QbertGameState::PAUSE:
		dae::SceneManager::GetInstance().GetScene("GameScene")->Pause();
		dae::SceneManager::GetInstance().GetScene("HudScene")->Pause();
		dae::SceneManager::GetInstance().LoadScene("InGameMenu");
		break;
	case QbertGameState::END:
		break;
	default:
		break;
	}
	m_PreviousState = m_State;
}
