#include "pch.h"
#include "MainLevel.h"

#include <InputManager.h>
#include <SceneManager.h>
#include <ResourceManager.h>

#include <UIComponent.h>
#include "Spawner.h"

#include "PlayerController.h"

#include "GameObservers.h"

#include <ServiceLocator.h>
#include "QbertGameController.h"
#include "InGameMenuUI.h"

void MainLevel::Initialize()
{
	auto gameController = (QbertGameController&)ServiceLocator::GetGameController();
	auto& input = InputManager::GetInstance();
	//ui object
	auto font16 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto font32 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);

	m_pGameScene = SceneManager::GetInstance().CreateScene("GameScene");
	m_pHUDScene = SceneManager::GetInstance().CreateScene("HudScene");
	m_pInGameMenuScene = SceneManager::GetInstance().CreateScene("InGameMenu");

	////UI 1
	auto go = std::make_shared<GameObject>();
	go->SetPosition(0, 10);
	auto ui = go->AddComponent<UIComponent>();
	//player 1
	auto text = ui->AddTextComponent("Player1txt", "Player 1", font32);
	//score 1
	text = ui->AddTextComponent("Score", "0", font16, 0, 50);
	std::shared_ptr<ScoreDisplay> scoreDisplayP1 = std::make_shared<ScoreDisplay>(text);
	//lives 1
	text = ui->AddTextComponent("Lives", "LIVES: 3", font16, 0, 65);
	std::shared_ptr<LivesDisplay> livesDisplayP1 = std::make_shared<LivesDisplay>(text);
	m_pHUDScene->Add(go);


	//level
	go = std::make_shared<GameObject>();
	if (gameController.GetQbertGameMode() != GameMode::COOP)
		go->SetPosition(500, 10);
	else
		go->SetPosition(250, 10);
	ui = go->AddComponent<UIComponent>();
	text = ui->AddTextComponent("Level", "LEVEL: 1", font32);
	std::shared_ptr<LevelDisplay> levelDisplay = std::make_shared<LevelDisplay>(text);
	//round
	text = ui->AddTextComponent("Round", "ROUND: 1", font32, 0, 30);
	std::shared_ptr<RoundsDisplay> roundDisplay = std::make_shared<RoundsDisplay>(text);
	m_pHUDScene->Add(go);

	//grid
	std::shared_ptr<EnemyFall> fallObserver = std::make_shared<EnemyFall>();
	auto gridObj = std::make_shared<GameObject>();
	auto grid = gridObj->AddComponent<Grid>();
	grid->AddObserver(levelDisplay);
	grid->AddObserver(roundDisplay);
	grid->AddObserver(fallObserver);
	grid->SetTopPiramid(9, 3);
	LevelReader::GetInstance().Read(1, 1);
	m_pGameScene->Add(gridObj);

	//player 1
	go = std::make_shared<GameObject>();
	auto player = go->AddComponent<Player>();
	player->SetGrid(grid);
	if (gameController.GetQbertGameMode() != GameMode::COOP)
		player->SetStartLocation(9, 3);
	else
		player->SetStartLocation(3, 9);

	player->AddObserver(livesDisplayP1);
	player->AddObserver(scoreDisplayP1);
	std::vector<std::shared_ptr<Player>> players;
	players.push_back(player);
	m_pGameScene->Add(go);

	int playerIndex = 0;
	//controller input player 1
	input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(players[playerIndex]), playerIndex);

	//keyboard input player 1
	input.AddInput(KeyboardKeys::W, std::make_unique<JumpTopRight>(players[playerIndex]));
	input.AddInput(KeyboardKeys::D, std::make_unique<JumpBottomRight>(players[playerIndex]));
	input.AddInput(KeyboardKeys::S, std::make_unique<JumpBottomLeft>(players[playerIndex]));
	input.AddInput(KeyboardKeys::A, std::make_unique<JumpTopLeft>(players[playerIndex]));

	input.AddInput(KeyboardKeys::Esc, std::make_unique<InGameMenu>(players[0]));

	//player 2
#pragma region SETUP_PLAYER2
	if (gameController.GetQbertGameMode() == GameMode::COOP)
	{
		////UI 2
		go = std::make_shared<GameObject>();
		go->SetPosition(500, 10);
		ui = go->AddComponent<UIComponent>();
		//player 2
		text = ui->AddTextComponent("Player2txt", "Player 2", font32);
		//score 2
		text = ui->AddTextComponent("Score", "0", font16, 0, 50);
		std::shared_ptr<ScoreDisplay> scoreDisplayP2 = std::make_shared<ScoreDisplay>(text);
		//lives 2
		text = ui->AddTextComponent("Lives", "LIVES: 3", font16, 0, 65);
		std::shared_ptr<LivesDisplay> livesDisplayP2 = std::make_shared<LivesDisplay>(text);
		m_pHUDScene->Add(go);

		go = std::make_shared<GameObject>();
		player = go->AddComponent<Player>();
		player->SetGrid(grid);
		player->SetStartLocation(15, 9);

		player->AddObserver(livesDisplayP2);
		player->AddObserver(scoreDisplayP2);
		players.push_back(player);
		m_pGameScene->Add(go);

		int playerIndex = 1;
		//controller input player 2
		input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(players[playerIndex]), playerIndex);

		//keyboard input player 2
		input.AddInput(KeyboardKeys::Up, std::make_unique<JumpTopRight>(players[playerIndex]));
		input.AddInput(KeyboardKeys::Right, std::make_unique<JumpBottomRight>(players[playerIndex]));
		input.AddInput(KeyboardKeys::Down, std::make_unique<JumpBottomLeft>(players[playerIndex]));
		input.AddInput(KeyboardKeys::Left, std::make_unique<JumpTopLeft>(players[playerIndex]));
	}
#pragma endregion

	grid->ConstructPiramid(players);
	grid->ConstructDiscs();


	go = std::make_shared<GameObject>();
	auto spawner = go->AddComponent<Spawner>();
	spawner->SetGrid(grid);
	spawner->SetPlayers(players);
	spawner->LoadLevelData();
	std::shared_ptr<PlayerDeath> playerDeath = std::make_shared<PlayerDeath>(spawner);
	players[0]->AddObserver(playerDeath);
	if (gameController.GetQbertGameMode() == GameMode::COOP)
		players[1]->AddObserver(playerDeath);

#pragma region InGameMenu
	go = std::make_shared<GameObject>();
	go->AddComponent<InGameMenuUI>();
	m_pInGameMenuScene->Add(go);
#pragma endregion

	dae::SceneManager::GetInstance().LoadScene("GameScene");
	dae::SceneManager::GetInstance().LoadScene("HudScene");
}

void MainLevel::Update()
{
	//if (m_Grid != nullptr)
	//{
	//	//freeze time
	//	if (GameContext::GetInstance().GetGameState() == GameState::GREENBALL)
	//	{
	//		m_FreezeTimer += Time::GetInstance().GetElapsedTime();
	//		if (m_FreezeTimer >= m_FreezeTime)
	//		{
	//			m_FreezeTimer = 0.f;
	//			GameContext::GetInstance().Play();
	//		}
	//		else
	//			return;
	//	}
	//	//win time
	//	else if (GameContext::GetInstance().GetGameState() == GameState::WIN)
	//	{
	//		m_Timer += Time::GetInstance().GetElapsedTime();
	//		if (m_Timer >= m_WinTime)
	//		{
	//			m_Timer = 0.f;
	//			//you can win in freeze mode -> reset time
	//			m_FreezeTimer = 0.f;
	//			GameContext::GetInstance().Play();
	//		}
	//		else
	//			return;
	//	}
	//	//new level
	//	if (m_Grid->NewLevel())
	//	{
	//		int lv = m_Grid->GetCurrentLevel();
	//		int rnd = m_Grid->GetCurrentRound();
	//		LevelReader::GetInstance().Read(lv, rnd);
	//		m_Grid->NewPiramid();
	//		m_Grid->NewDiscs();
	//		for (auto player : m_Players)
	//			player->Reset();
	//		m_Spawner->Clear();
	//		m_Spawner->LoadLevelData();
	//	}
	//	m_Spawner->Update();
	//}
}

void MainLevel::Deactivate()
{
	dae::SceneManager::GetInstance().UnloadScene("GameScene");
	dae::SceneManager::GetInstance().UnloadScene("HudScene");
	dae::SceneManager::GetInstance().UnloadScene("InGameMenu");
}
