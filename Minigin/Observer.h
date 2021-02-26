#pragma once
#include "GameObject.h"
#include "TextComponent.h"

class PlayerComponent;
class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(std::shared_ptr<PlayerComponent> actor, const std::string& event) = 0;
};

class LivesDisplay : public Observer
{
public:
	explicit LivesDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject } {}

	void OnNotify(std::shared_ptr<PlayerComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};

class ScoreDisplay : public Observer
{
public:
	explicit ScoreDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject}{}

	void OnNotify(std::shared_ptr<PlayerComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};