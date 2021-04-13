#include "pch.h"
#include "GameObservers.h"
#include "Player.h"
#include <ServiceLocator.h>
#include "Grid.h"

void LivesDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "IS_DEAD")
	{
		std::cout << "PlayerDied" << std::endl;
		m_UI->SetText("Lives: " + std::to_string(std::dynamic_pointer_cast<Player>(actor)->GetHealth()));
	}
}

void ScoreDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "SCORE_ADDED")
	{
		std::cout << "SCORE_ADDED" << std::endl;
		m_UI->SetText(std::to_string(std::dynamic_pointer_cast<Player>(actor)->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
}

void LevelDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "NEW_LEVEL")
	{
		std::cout << "NEW LEVEL" << std::endl;
		m_UI->SetText(std::to_string(std::dynamic_pointer_cast<Grid>(actor)->GetCurrentLevel()));
	}
}

void RoundsDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "NEW_ROUND")
	{
		std::cout << "NEW ROUND" << std::endl;
		m_UI->SetText(std::to_string(std::dynamic_pointer_cast<Grid>(actor)->GetCurrentRound()));
	}
}
