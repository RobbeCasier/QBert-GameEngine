#pragma once
#include <Commands.h>
#include "Coily.h"
#include "Ugg_Wrongway.h"
#include "Slick_Sam.h"
#include "RedBall.h"

class CoilyJumpBottomLeft final : public Command
{
public:
	explicit CoilyJumpBottomLeft(std::shared_ptr<Player> actor) : Command(actor) {}
	void Execute() override
	{
		auto actor = std::dynamic_pointer_cast<Coily>(m_Actor);
		if (actor->IsPossesed())
			actor->Jump(-1, 1);
	}
};
class CoilyJumpBottomRight final : public Command
{
public:
	explicit CoilyJumpBottomRight(std::shared_ptr<Player> actor) : Command(actor) {}
	void Execute() override
	{
		auto actor = std::dynamic_pointer_cast<Coily>(m_Actor);
		if (actor->IsPossesed())
			actor->Jump(1, 1);
	}
};
class CoilyJumpTopLeft final : public Command
{
public:
	explicit CoilyJumpTopLeft(std::shared_ptr<Player> actor) : Command(actor) {}
	void Execute() override
	{
		auto actor = std::dynamic_pointer_cast<Coily>(m_Actor);
		if (actor->IsPossesed())
			actor->Jump(-1, -1);
	}
};
class CoilyJumpTopRight final : public Command
{
public:
	explicit CoilyJumpTopRight(std::shared_ptr<Coily> actor) : Command(actor) {}
	void Execute() override
	{
		auto actor = std::dynamic_pointer_cast<Coily>(m_Actor);
		if (actor->IsPossesed())
			actor->Jump(1, -1);
	}
};

class CoilyFall final : public Command
{
public:
	explicit CoilyFall(std::shared_ptr<PlayerComponent> coily) : Command(coily){}
	void Execute() override
	{
		ServiceLocator::GetSoundSystem().Play(8, 100);
		std::dynamic_pointer_cast<Coily>(m_Actor)->Fall();
	}
};

class UggAndWrongwayFall final : public Command
{
public:
	explicit UggAndWrongwayFall(std::shared_ptr<BaseComponent> uw) : Command(uw) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Ugg_Wrongway>(m_Actor)->Fall();
	}
};

class SlickAndSamFall final : public Command
{
public:
	explicit SlickAndSamFall(std::shared_ptr<BaseComponent> ss) : Command(ss){}
	void Execute() override
	{
		std::dynamic_pointer_cast<Slick_Sam>(m_Actor)->Fall();
	}
};

class RedBallFall final : public Command
{
public:
	explicit RedBallFall(std::shared_ptr<BaseComponent> rb) : Command(rb) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<RedBall>(m_Actor)->Fall();
	}
};