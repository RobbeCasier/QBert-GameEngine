#pragma once
#include <Scene.h>

struct SDL_Window;

class MainScene :
    public dae::Scene
{
public:
    MainScene(const std::string& name) : dae::Scene(name) {};
    virtual ~MainScene() = default;
    void Initialize() override;
    void Update() override;
    void Render(SDL_Window* window) const override;
private:
};

