#pragma once
#include <Level.h>
#include <Scene.h>

class MainMenuLevel final:
    public dae::Level
{
public:
    MainMenuLevel() : dae::Level{"MainMenuLevel"}{}
    virtual ~MainMenuLevel() = default;
    void Initialize() override;
    void Update() override;
    void Deactivate() override;
private:
    std::shared_ptr<dae::Scene> m_pMainMenuScene;
};

