#include "pch.h"
#include "LevelScene.h"
#include <ResourceManager.h>
#include <GameObject.h>
#include <UIComponent.h>
#include "Player.h"
#include "Coily.h"
#include <InputManager.h>
#include "PlayerController.h"
#include "GameObservers.h"
#include <ServiceLocator.h>
#include "Spawner.h"
#include <GameContext.h>
#include <GameTime.h>

void LevelScene::Initialize()
{
	auto& input = InputManager::GetInstance();
	//ui object
	auto font16 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto font32 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);

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
	Add(go);


	//level
	go = std::make_shared<GameObject>();
	if (!m_IsCoop)
		go->SetPosition(500, 10);
	else
		go->SetPosition(250, 10);
	ui = go->AddComponent<UIComponent>();
	text = ui->AddTextComponent("Level", "LEVEL: 1", font32);
	std::shared_ptr<LevelDisplay> levelDisplay = std::make_shared<LevelDisplay>(text);
	//round
	text = ui->AddTextComponent("Round", "ROUND: 1", font32, 0, 30);
	std::shared_ptr<RoundsDisplay> roundDisplay = std::make_shared<RoundsDisplay>(text);
	Add(go);



	//grid
	auto gridObj = std::make_shared<GameObject>();
	m_Grid = gridObj->AddComponent<Grid>();
	m_Grid->AddObserver(levelDisplay);
	m_Grid->AddObserver(roundDisplay);
	m_Grid->SetTopPiramid(9, 3);
	m_Reader.Read("../Data/Levels/1_1.txt");
	Add(gridObj);

	//player 1
	go = std::make_shared<GameObject>();
	auto player = go->AddComponent<Player>();
	player->SetGrid(m_Grid);
	if (!m_IsCoop)
		player->SetStartLocation(9, 3);
	else
		player->SetStartLocation(3, 9);

	player->AddObserver(livesDisplayP1);
	player->AddObserver(scoreDisplayP1);
	m_Players.push_back(player);
	Add(go);

	int playerIndex = 0;
	//controller input player 1
	input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(m_Players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(m_Players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(m_Players[playerIndex]), playerIndex);
	input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(m_Players[playerIndex]), playerIndex);

	//keyboard input player 1
	input.AddInput(KeyboardKeys::W, std::make_unique<JumpTopRight>(m_Players[playerIndex]));
	input.AddInput(KeyboardKeys::D, std::make_unique<JumpBottomRight>(m_Players[playerIndex]));
	input.AddInput(KeyboardKeys::S, std::make_unique<JumpBottomLeft>(m_Players[playerIndex]));
	input.AddInput(KeyboardKeys::A, std::make_unique<JumpTopLeft>(m_Players[playerIndex]));

	//player 2
#pragma region SETUP_PLAYER2
	if (m_IsCoop)
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
		Add(go);

		go = std::make_shared<GameObject>();
		player = go->AddComponent<Player>();
		player->SetGrid(m_Grid);
		player->SetStartLocation(15, 9);

		player->AddObserver(livesDisplayP2);
		player->AddObserver(scoreDisplayP2);
		m_Players.push_back(player);
		Add(go);

		int playerIndex = 1;
		//controller input player 2
		input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(m_Players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(m_Players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(m_Players[playerIndex]), playerIndex);
		input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(m_Players[playerIndex]), playerIndex);

		//keyboard input player 2
		input.AddInput(KeyboardKeys::Up, std::make_unique<JumpTopRight>(m_Players[playerIndex]));
		input.AddInput(KeyboardKeys::Right, std::make_unique<JumpBottomRight>(m_Players[playerIndex]));
		input.AddInput(KeyboardKeys::Down, std::make_unique<JumpBottomLeft>(m_Players[playerIndex]));
		input.AddInput(KeyboardKeys::Left, std::make_unique<JumpTopLeft>(m_Players[playerIndex]));
	}
#pragma endregion

	m_Grid->ConstructPiramid(m_Players, (SideColor)m_Reader.GetColor(), m_Reader.GetOrder());
	m_Grid->ConstructDiscs((SideColor)m_Reader.GetColor(), m_Reader.GetDiscs());



	m_Spawner = std::make_shared<Spawner>(shared_from_this(), m_Grid, m_Players);
	m_Spawner->Initialize();
	std::shared_ptr<PlayerDeath> playerDeath = std::make_shared<PlayerDeath>(m_Spawner);
	m_Players[0]->AddObserver(playerDeath);
	if (m_IsCoop)
		m_Players[1]->AddObserver(playerDeath);
}

void LevelScene::Update()
{
	if (m_Grid != nullptr)
	{
		if (GameContext::GetInstance().GetGameState() == GameState::WIN)
		{
			m_Timer += Time::GetInstance().GetElapsedTime();
			if (m_Timer >= m_WinTime)
			{
				m_Timer = 0.f;
				GameContext::GetInstance().Play();
			}
			else
				return;
		}
		if (m_Grid->NewLevel())
		{
			int lv = m_Grid->GetCurrentLevel();
			int rnd = m_Grid->GetCurrentRound();
			std::string file = "../Data/Levels/" + std::to_string(lv) + "_" + std::to_string(rnd) + ".txt";
			m_Reader.Read(file);
			m_Grid->NewPiramid((GameType)m_Reader.GetStyle(), (SideColor)m_Reader.GetColor(), m_Reader.GetOrder());
			m_Grid->NewDiscs((SideColor)m_Reader.GetColor(), m_Reader.GetDiscs());
			for (auto player : m_Players)
				player->Reset();
			m_Spawner->Clear();
		}
		m_Spawner->Update();
	}
}

void LevelScene::Render(SDL_Window* window) const
{
	UNREFERENCED_PARAMETER(window);
}
