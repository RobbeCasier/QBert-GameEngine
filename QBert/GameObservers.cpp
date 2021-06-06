#include "pch.h"
#include "GameObservers.h"
#include <GameContext.h>
#include "Player.h"
#include <ServiceLocator.h>
#include "Grid.h"
#include "Spawner.h"

void LivesDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "IS_DEAD")
	{
		m_UI->SetText("LIVES: " + std::to_string(std::dynamic_pointer_cast<Player>(actor)->GetHealth()));
	}
}

void ScoreDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "SCORE_ADDED")
	{
		m_UI->SetText(std::to_string(std::dynamic_pointer_cast<Player>(actor)->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
}

void LevelDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "NEW_LEVEL")
	{
		m_UI->SetText("LEVEL: " + std::to_string(std::dynamic_pointer_cast<Grid>(actor)->GetCurrentLevel()));
	}
}

void RoundsDisplay::OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event)
{
	if (event == "NEW_ROUND")
	{
		GameContext::GetInstance().Win();

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
	else if (event == "KILL_RB")
	{
		m_Spawner->KillRB(enemy);
	}
	else if (event == "KILL_GB")
	{
		m_Spawner->KillGB();
	}
}

void PlayerDeath::OnNotify(std::shared_ptr<BaseComponent> player, const std::string& event)
{
	if (event == "CLEAR_ENEMIES")
	{
		m_Spawner->Clear();
	}
}

void EnemyFall::OnNotify(std::shared_ptr<BaseComponent> enemy, const std::string& event)
{
	if (event == "FALL_COILY")
	{
		std::dynamic_pointer_cast<Coily>(enemy)->Fall();
	}
	else if (event == "FALL_UW")
	{
		std::dynamic_pointer_cast<Ugg_Wrongway>(enemy)->Fall();
	}
	else if (event == "FALL_SS")
	{
		std::dynamic_pointer_cast<Slick_Sam>(enemy)->Fall();
	}
	else if (event == "FALL_RB")
	{
		std::dynamic_pointer_cast<RedBall>(enemy)->Fall();
	}
	else if (event == "FALL_GB")
	{
		std::dynamic_pointer_cast<GreenBall>(enemy)->Fall();
	}
}
