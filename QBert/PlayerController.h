#pragma once
#include <Commands.h>
#include "Player.h"
#include <ServiceLocator.h>
#include <GameContext.h>
#include "LevelReader.h"
#include "QbertGameController.h"

class JumpBottomLeft final : public Command
{
public:
	explicit JumpBottomLeft(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->Jump(-1, 1);
	}
};
class JumpBottomRight final : public Command
{
public:
	explicit JumpBottomRight(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->Jump(1, 1);
	}
};
class JumpTopLeft final : public Command
{
public:
	explicit JumpTopLeft(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->Jump(-1, -1);
	}
};
class JumpTopRight final : public Command
{
public:
	explicit JumpTopRight(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->Jump(1, -1);
	}
};

class InGameMenu final : public Command
{
public:
	explicit InGameMenu(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		if (qgc.GetQbertState() == QbertGameState::LEVEL)
			qgc.SetQbertState(QbertGameState::PAUSE);
		else if (qgc.GetQbertState() == QbertGameState::PAUSE)
			qgc.SetQbertState(QbertGameState::LEVEL);
	}
};

//Death commands
class Death final : public Command
{
public:
	explicit Death(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		GameContext::GetInstance().Collide();
		std::dynamic_pointer_cast<Player>(m_Actor)->DecreaseHealth();
	}
};
class Fall final : public Command
{
public:
	explicit Fall(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		ServiceLocator::GetSoundSystem().Play(7, 100);
		std::dynamic_pointer_cast<Player>(m_Actor)->Fall();
		std::dynamic_pointer_cast<Player>(m_Actor)->DecreaseHealth();
	}
};

class Lift final : public Command
{
public:
	explicit Lift(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		ServiceLocator::GetSoundSystem().Play(6, 100);
		std::dynamic_pointer_cast<Player>(m_Actor)->Lift();
	}
};

class ChangeColorPlayerFinal final :public Command
{
public:
	explicit ChangeColorPlayerFinal(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(25);
	}
};

class ChangeColorPlayerIntermediat final :public Command
{
public:
	explicit ChangeColorPlayerIntermediat(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(15);
	}
};

class BeatCoily final :public Command
{
public:
	explicit BeatCoily(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(500);
	}
};

class RemainingDisc final :public Command
{
public:
	explicit RemainingDisc(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(50);
	}
};

class Catch final :public Command
{
public:
	explicit Catch(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(300);
	}
};

class Bonus final :public Command
{
public:
	explicit Bonus(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(LevelReader::GetInstance().GetLevelParamters().bonus);
	}
};