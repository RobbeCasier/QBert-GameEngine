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

void LevelScene::Initialize()
{
	auto& input = InputManager::GetInstance();
	//ui object
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	////ui 1
	auto go = std::make_shared<GameObject>();
	go->SetPosition(0, 50);
	auto ui = go->AddComponent<UIComponent>();
	//lives 1
	auto text = ui->AddTextComponent("Lives", "LIVES: 3", font);
	std::shared_ptr<LivesDisplay> livesDisplayP1 = std::make_shared<LivesDisplay>(text);
	//score
	text = ui->AddTextComponent("Score", "0", font, 0, 15);
	std::shared_ptr<ScoreDisplay> scoreDisplayP1 = std::make_shared<ScoreDisplay>(text);
	Add(go);

	//level
	go = std::make_shared<GameObject>();
	go->SetPosition(500, 50);
	ui = go->AddComponent<UIComponent>();
	text = ui->AddTextComponent("Level", "LEVEL: 1", font);
	std::shared_ptr<LevelDisplay> levelDisplay = std::make_shared<LevelDisplay>(text);
	//round
	text = ui->AddTextComponent("Round", "ROUND: 1", font, 0, 15);
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
	m_Player = go->AddComponent<Player>();
	m_Player->SetGrid(m_Grid);
	m_Player->SetStartLocation(9, 3);

	m_Player->AddObserver(livesDisplayP1);
	m_Player->AddObserver(scoreDisplayP1);
	Add(go);

	m_Grid->ConstructPiramid(m_Player, (SideColor)m_Reader.GetColor(), m_Reader.GetOrder());
	m_Grid->ConstructDiscs((SideColor)m_Reader.GetColor(), m_Reader.GetDiscs());

	//add a new controller to the 
	input.AddController();
	//controller input
	input.AddInput(ControllerButton::ButtonUP, std::make_unique<JumpTopRight>(m_Player), 0);
	input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<JumpBottomRight>(m_Player), 0);
	input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<JumpBottomLeft>(m_Player), 0);
	input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<JumpTopLeft>(m_Player), 0);

	//keyboard input
	input.AddInput(KeyboardKeys::W, std::make_unique<JumpTopRight>(m_Player));
	input.AddInput(KeyboardKeys::D, std::make_unique<JumpBottomRight>(m_Player));
	input.AddInput(KeyboardKeys::S, std::make_unique<JumpBottomLeft>(m_Player));
	input.AddInput(KeyboardKeys::A, std::make_unique<JumpTopLeft>(m_Player));

	m_Spawner = std::make_shared<Spawner>(shared_from_this(), m_Grid, m_Player);
	m_Spawner->Initialize();
	std::shared_ptr<PlayerDeath> playerDeath = std::make_shared<PlayerDeath>(m_Spawner);
	m_Player->AddObserver(playerDeath);
}

void LevelScene::Update()
{
	if (m_Grid != nullptr)
	{
		if (m_Grid->NewLevel())
		{
			int lv = m_Grid->GetCurrentLevel();
			int rnd = m_Grid->GetCurrentRound();
			std::string file = "../Data/Levels/" + std::to_string(lv) + "_" + std::to_string(rnd) + ".txt";
			m_Reader.Read(file);
			m_Grid->NewPiramid((GameType)m_Reader.GetStyle(), (SideColor)m_Reader.GetColor(), m_Reader.GetOrder());
			m_Grid->NewDiscs((SideColor)m_Reader.GetColor(), m_Reader.GetDiscs());
			m_Player->SetStartLocation(9, 3);
			m_Spawner->Clear();
		}
		m_Spawner->Update();
	}
}

void LevelScene::Render(SDL_Window* window) const
{
	UNREFERENCED_PARAMETER(window);
}
