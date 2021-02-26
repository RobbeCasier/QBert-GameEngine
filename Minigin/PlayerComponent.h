#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class PlayerComponent :
    public BaseComponent, public std::enable_shared_from_this<PlayerComponent>
{
public:
    explicit PlayerComponent() = default;
    virtual ~PlayerComponent() = default;

    virtual void Initialize() override;
    virtual void Update() override {};

    void AddObserver(std::shared_ptr<Observer> observer);
    
    void DecreaseHealth();
    void AddScoreChangeColor();
    void AddScoreBeatCoily();
    void AddScoreRemainingDisc();
    void AddScoreCatch();

    int GetHealth() const { return m_Health; }
    int GetScore() const { return m_Score; }
private:
    int m_Health = 3;
    int m_Score = 0;
    std::unique_ptr<Subject> m_ActorChanged;
};

