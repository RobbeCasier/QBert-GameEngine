#pragma once
#include <Scene.h>
#include <backends\imgui_impl_opengl2.h>
#include "Grid.h"
#include "LevelReader.h"

class Spawner;
class Coily;
struct SDL_Window;
class LevelScene :
    public dae::Scene
{
public:
    LevelScene(const std::string& name) : dae::Scene(name) {};
    virtual ~LevelScene() = default;
    void Initialize() override;
    void Update() override;
    void Render(SDL_Window * window) const override;

    void SetCoop() { m_IsCoop = true; }
    void SetMultiplayer() { m_IsMultiplayer = true; }
private:
    std::shared_ptr<Grid> m_Grid;
    std::vector<std::shared_ptr<Player>> m_Players;
    std::shared_ptr<Spawner> m_Spawner;
    LevelReader m_Reader;

    float m_WinTime = 2.5f;
    float m_FreezeTime = 10.f;
    float m_Timer = 0.f;
    float m_FreezeTimer = 0.f;

    bool m_IsCoop = false;
    bool m_IsMultiplayer = false;
};

