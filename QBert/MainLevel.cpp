#include "pch.h"
#include "MainLevel.h"

#include <InputManager.h>
#include <SceneManager.h>
#include <ResourceManager.h>
#include "PlayerManager.h"

#include <UIComponent.h>
#include "Spawner.h"

#include "PlayerController.h"
#include "EnemyController.h"

#include "GameObservers.h"

#include <ServiceLocator.h>
#include "QbertGameController.h"
#include "InGameMenuUI.h"
#include <GameTime.h>
#include <FPS.h>
#include "Grid.h"


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
	m_pHUDScene->AddObject(go);


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
	m_pHUDScene->AddObject(go);

	//grid
	std::shared_ptr<EnemyFall> fallObserver = std::make_shared<EnemyFall>();
	auto gridObj = std::make_shared<GameObject>();
	m_pGrid = gridObj->AddComponent<Grid>();
	m_pGrid->AddObserver(levelDisplay);
	m_pGrid->AddObserver(roundDisplay);
	m_pGrid->AddObserver(fallObserver);
	m_pGrid->SetTopPiramid(9, 3);
	LevelReader::GetInstance().Read(1, 1);
	m_pGameScene->AddObject(gridObj);

	//player 1
	go = std::make_shared<GameObject>();
	auto player = go->AddComponent<Player>();
	player->SetGrid(m_pGrid);
	if (gameController.GetQbertGameMode() != GameMode::COOP)
		player->SetStartLocation(9, 3);
	else
		player->SetStartLocation(3, 9);

	player->AddObserver(livesDisplayP1);
	player->AddObserver(scoreDisplayP1);

	PlayerManager::GetInstance().AddPlayer(player);
	m_pGameScene->AddObject(go);

	int playerIndex = 0;
	input.AddController();
	input.AddController();
	//controller input player 1
	input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(player), playerIndex);
	input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(player), playerIndex);
	input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(player), playerIndex);
	input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(player), playerIndex);

	//keyboard input player 1
	input.AddInput(KeyboardKeys::W, std::make_unique<JumpTopRight>(player));
	input.AddInput(KeyboardKeys::D, std::make_unique<JumpBottomRight>(player));
	input.AddInput(KeyboardKeys::S, std::make_unique<JumpBottomLeft>(player));
	input.AddInput(KeyboardKeys::A, std::make_unique<JumpTopLeft>(player));

	input.AddInput(KeyboardKeys::Esc, std::make_unique<InGameMenu>(player));

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
		m_pHUDScene->AddObject(go);

		go = std::make_shared<GameObject>();

			player = go->AddComponent<Player>();
			player->SetGrid(m_pGrid);
			player->AddObserver(livesDisplayP2);
			player->AddObserver(scoreDisplayP2);
			player->SetStartLocation(15, 9);
			
			PlayerManager::GetInstance().AddPlayer(player);
			m_pGameScene->AddObject(go);

		int playerIndex = 1;
			//controller input player 2
			input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(player), playerIndex);
			input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(player), playerIndex);
			input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(player), playerIndex);
			input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(player), playerIndex);

			//keyboard input player 2
			input.AddInput(KeyboardKeys::Up, std::make_unique<JumpTopRight>(player));
			input.AddInput(KeyboardKeys::Right, std::make_unique<JumpBottomRight>(player));
			input.AddInput(KeyboardKeys::Down, std::make_unique<JumpBottomLeft>(player));
			input.AddInput(KeyboardKeys::Left, std::make_unique<JumpTopLeft>(player));
	}
#pragma endregion

	m_pGrid->ConstructPiramid();

	go = std::make_shared<GameObject>();
	m_pGameScene->AddObject(go);
	m_pSpawner = go->AddComponent<Spawner>();
	m_pSpawner->SetGrid(m_pGrid);
	m_pSpawner->LoadLevelData();
	std::shared_ptr<PlayerDeath> playerDeath = std::make_shared<PlayerDeath>(m_pSpawner);
	auto players = PlayerManager::GetInstance().GetPlayers();
	players[0]->AddObserver(playerDeath);
	if (gameController.GetQbertGameMode() == GameMode::COOP)
		players[1]->AddObserver(playerDeath);

#pragma region InGameMenuScene
	go = std::make_shared<GameObject>();
	go->AddComponent<InGameMenuUI>();
	m_pInGameMenuScene->AddObject(go);
#pragma endregion
}

void MainLevel::Update()
{
	if (PlayerManager::GetInstance().GetPlayers().size() == 0)
		((QbertGameController&)ServiceLocator::GetGameController()).SetQbertState(QbertGameState::GAMEOVER);
	//freeze time
	if (GameContext::GetInstance().GetGameState() == GameState::GREENBALL)
	{
		m_FreezeTimer += Time::GetInstance().GetElapsedTime();
		if (m_FreezeTimer >= m_FreezeTime)
		{
			m_FreezeTimer = 0.f;
			GameContext::GetInstance().Play();
		}
		else
			return;
	}
	//win time
	else if (GameContext::GetInstance().GetGameState() == GameState::WIN)
	{
		m_Timer += Time::GetInstance().GetElapsedTime();
		if (m_Timer >= m_WinTime)
		{
			m_Timer = 0.f;
			//you can win in freeze mode -> reset time
			m_FreezeTimer = 0.f;
			GameContext::GetInstance().Play();
		}
		else
			return;
	}
		//new level
	if (m_pGrid && m_pGrid->NewLevel())
	{
		int lv = m_pGrid->GetCurrentLevel();
		int rnd = m_pGrid->GetCurrentRound();
		m_pGrid->CollectDiscs();
		if (!LevelReader::GetInstance().Read(lv, rnd))
		{
			((QbertGameController&)ServiceLocator::GetGameController()).SetQbertState(QbertGameState::END);
			return;
		}
		m_pGrid->NewPiramid();
		m_pSpawner->Clear();
		m_pSpawner->LoadLevelData();
	}
	
}

void MainLevel::Deactivate()
{
	dae::SceneManager::GetInstance().RemoveScene("GameScene");
	dae::SceneManager::GetInstance().RemoveScene("HudScene");
	dae::SceneManager::GetInstance().RemoveScene("InGameMenu");
}
