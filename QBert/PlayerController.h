#pragma once
#include <Commands.h>
#include "Player.h"

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
class Death final : public Command
{
public:
	explicit Death(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->DecreaseHealth();
	}
};

class ChangeColorPlayer final :public Command
{
public:
	explicit ChangeColorPlayer(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		std::dynamic_pointer_cast<Player>(m_Actor)->AddScore(25);
	}
};

//class BeatCoily final :public Command
//{
//public:
//	explicit BeatCoily(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
//	void Execute() override
//	{
//		m_Actor->AddScoreBeatCoily();
//	}
//};
//
//class RemainingDisc final :public Command
//{
//public:
//	explicit RemainingDisc(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
//	void Execute() override
//	{
//		m_Actor->AddScoreRemainingDisc();
//	}
//};
//
//class Catch final :public Command
//{
//public:
//	explicit Catch(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
//	void Execute() override
//	{
//		m_Actor->AddScoreCatch();
//	}
//};