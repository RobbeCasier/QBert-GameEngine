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
private:
    std::shared_ptr<Grid> m_Grid;
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<Coily> m_Coily;
    std::shared_ptr<Spawner> m_Spawner;
    LevelReader m_Reader;
};

