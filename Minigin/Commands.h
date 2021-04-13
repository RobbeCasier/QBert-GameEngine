#pragma once
#include "GameObject.h"
#include "PlayerComponent.h"

class Command
{
public:
	explicit Command(std::shared_ptr<BaseComponent> actor) : m_Actor{ actor }{}
	virtual ~Command() { m_Actor = nullptr; };
	virtual void Execute() = 0;
protected:
	std::shared_ptr<BaseComponent> GetActor() const { return m_Actor; }
	std::shared_ptr<BaseComponent> m_Actor;
private:
};