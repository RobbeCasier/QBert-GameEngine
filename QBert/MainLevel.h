#pragma once
#include <Level.h>
#include <Scene.h>

class Grid;
class Spawner;
class QbertGameController;
class MainLevel final : public dae::Level
{
public:
	MainLevel() : dae::Level{ "Game" }{};
	virtual ~MainLevel() = default;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Deactivate() override;

private:
	std::shared_ptr<dae::Scene> m_pGameScene;
	std::shared_ptr<dae::Scene> m_pInGameMenuScene;
	std::shared_ptr<dae::Scene> m_pHUDScene;

	std::shared_ptr<Grid> m_pGrid;
	std::shared_ptr<Spawner> m_pSpawner;

	float m_Timer = 0.f;
	float m_WinTime = 2.0f;

	float m_FreezeTime = 8.0f;
	float m_FreezeTimer = 0.f;
};

