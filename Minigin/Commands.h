#pragma once
#include "GameObject.h"
#include "PlayerComponent.h"

class Command
{
public:
	explicit Command(std::shared_ptr<PlayerComponent> actor) : m_Actor{ actor }{}
	virtual ~Command() { m_Actor = nullptr; };
	virtual void Execute() = 0;
protected:
	std::shared_ptr<PlayerComponent> GetActor() const { return m_Actor; }
	std::shared_ptr<PlayerComponent> m_Actor;
private:
};

//instantDeath
class InstantDeath final : public Command
{
public:
	explicit InstantDeath(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		m_Actor->DecreaseHealth();
	}
};

class ChangeColor final :public Command
{
public:
	explicit ChangeColor(std::shared_ptr<PlayerComponent> actor) : Command(actor){}
	void Execute() override
	{
		m_Actor->AddScoreChangeColor();
	}
};

class BeatCoily final :public Command
{
public:
	explicit BeatCoily(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		m_Actor->AddScoreBeatCoily();
	}
};

class RemainingDisc final :public Command
{
public:
	explicit RemainingDisc(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		m_Actor->AddScoreRemainingDisc();
	}
};

class Catch final :public Command
{
public:
	explicit Catch(std::shared_ptr<PlayerComponent> actor) : Command(actor) {}
	void Execute() override
	{
		m_Actor->AddScoreCatch();
	}
};