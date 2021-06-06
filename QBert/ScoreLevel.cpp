#include "pch.h"
#include "ScoreLevel.h"
#include <GameObject.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include <ServiceLocator.h>
#include "QbertGameController.h"
#include <UIComponent.h>
#include "PlayerManager.h"
#include "EndUI.h"

void ScoreLevel::Initialize()
{
	m_pEndScene = dae::SceneManager::GetInstance().CreateScene("EndScene");
	auto controller = ((QbertGameController&)ServiceLocator::GetGameController());
	std::vector<int> scores = PlayerManager::GetInstance().GetScores();

	auto font24 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto font36 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font48 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 48);
	auto go = std::make_shared<GameObject>();
	go = std::make_shared<GameObject>();
	auto textComp = go->AddComponent<TextComponent>();

	//gamestate dependency
	if (controller.GetQbertState() == QbertGameState::GAMEOVER)
		textComp->SetText("GAME OVER");
	else if (controller.GetQbertState() == QbertGameState::END)
		textComp->SetText("YOU WIN");

	textComp->SetFont(font48);
	go->SetPosition(200, 20);
	m_pEndScene->AddObject(go);

	if (controller.GetQbertGameMode() != GameMode::COOP)
	{
		go = std::make_shared<GameObject>();
		go->SetPosition(300, 75);
		auto ui = go->AddComponent<UIComponent>();
		//score
		std::string score = std::to_string(scores[0]);
		auto text = ui->AddTextComponent("Score", score, font36, 0, 50);
		m_pEndScene->AddObject(go);
	}
	else
	{
		go = std::make_shared<GameObject>();
		go->SetPosition(150, 75);
		auto ui = go->AddComponent<UIComponent>();
		//player 1
		auto text = ui->AddTextComponent("Player1txt", "Player 1", font36);
		//score
		std::string score = std::to_string(scores[0]);
		text = ui->AddTextComponent("Score1", score, font24, 0, 50);
		m_pEndScene->AddObject(go);

		go = std::make_shared<GameObject>();
		go->SetPosition(450, 75);
		ui = go->AddComponent<UIComponent>();
		//player 1
		text = ui->AddTextComponent("Player2txt", "Player 2", font36);
		//score
		score = std::to_string(scores[1]);
		text = ui->AddTextComponent("Score2", score, font24, 0, 50);
		m_pEndScene->AddObject(go);
	}


	go = std::make_shared<GameObject>();
	go->AddComponent<EndUI>();
	m_pEndScene->AddObject(go);
}

void ScoreLevel::Deactivate()
{
	dae::SceneManager::GetInstance().RemoveScene("EndScene");
}
