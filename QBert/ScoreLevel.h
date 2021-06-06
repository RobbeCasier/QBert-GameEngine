#pragma once
#include <Level.h>
#include <Scene.h>

class ScoreLevel :
    public dae::Level
{
public:
    ScoreLevel() : dae::Level{"End"}{}
    virtual ~ScoreLevel() = default;
    virtual void Initialize() override;
    virtual void Update() override {};
    virtual void Deactivate() override;
private:
    std::shared_ptr<dae::Scene> m_pEndScene;
};

