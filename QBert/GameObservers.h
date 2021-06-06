#pragma once
#include <Observer.h>
#include <PlayerComponent.h>
#include <TextComponent.h>
#include "Grid.h"

class Spawner;

class LivesDisplay : public Observer
{
public:
	explicit LivesDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject } {}

	void OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};

class ScoreDisplay : public Observer
{
public:
	explicit ScoreDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject } {}

	void OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};

class LevelDisplay : public Observer
{
public:
	explicit LevelDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject } {}

	void OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};

class RoundsDisplay : public Observer
{
public:
	explicit RoundsDisplay(std::shared_ptr<TextComponent> uiObject) : m_UI{ uiObject } {}

	void OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event) override;
private:
	std::shared_ptr<TextComponent> m_UI;
};

class PlayerDeath : public Observer
{
public:
	explicit PlayerDeath(std::shared_ptr<Spawner> spawner) : m_Spawner(spawner){}

	void OnNotify(std::shared_ptr<BaseComponent> player, const std::string& event) override;

private:
	std::shared_ptr<Spawner> m_Spawner;
};

class EnemyDeath : public Observer
{
public:
	explicit EnemyDeath(std::shared_ptr<Spawner> spawner) : m_Spawner(spawner){}

	void OnNotify(std::shared_ptr<BaseComponent> enemy, const std::string& event) override;

private:
	std::shared_ptr<Spawner> m_Spawner;
};

class EnemyFall : public Observer
{
public:
	explicit EnemyFall() = default;

	void OnNotify(std::shared_ptr<BaseComponent> enemy, const std::string& event) override;
};

