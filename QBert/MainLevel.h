#pragma once
#include <Level.h>
#include <Scene.h>
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
};

