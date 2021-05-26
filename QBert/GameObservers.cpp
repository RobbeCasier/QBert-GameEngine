#include "pch.h"
#include "GameObservers.h"
#include "Player.h"
#include <ServiceLocator.h>
#include "Grid.h"
#include "Spawner.h"

void LivesDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "IS_DEAD")
	{
		std::cout << "PlayerDied" << std::endl;
		m_UI->SetText("LIVES: " + std::to_string(std::dynamic_pointer_cast<Player>(actor)->GetHealth()));
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
		m_UI->SetText("LEVEL: " + std::to_string(std::dynamic_pointer_cast<Grid>(actor)->GetCurrentLevel()));
		ServiceLocator::GetSoundSystem().Play(5, 100);
	}
}

void RoundsDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "NEW_ROUND")
	{
		std::cout << "NEW ROUND" << std::endl;
		m_UI->SetText("ROUND: " + std::to_string(std::dynamic_pointer_cast<Grid>(actor)->GetCurrentRound()));
		ServiceLocator::GetSoundSystem().Play(5, 100);
	}
}

void EnemyDeath::OnNotify(std::shared_ptr<BaseComponent> enemy, const std::string& event)
{
	if (event == "KILL_COILY")
	{
		m_Spawner->KillCoily();
	}
	else if (event == "KILL_UW")
	{
		m_Spawner->KillUW(enemy);
	}
	else if (event == "KILL_SS")
	{
		m_Spawner->KillSS(enemy);
	}
}

void PlayerDeath::OnNotify(std::shared_ptr<BaseComponent> player, const std::string& event)
{
	if (event == "CLEAR_ENEMIES")
	{
		m_Spawner->Clear();
	}
}
