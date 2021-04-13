#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class PlayerComponent :
    public BaseComponent, public std::enable_shared_from_this<PlayerComponent>
{
public:
    explicit PlayerComponent() { Initialize(); };
    virtual ~PlayerComponent() = default;

    virtual void Initialize() override;
    virtual void Update() override {};

    void SetPosition(glm::vec3 pos);

    void AddObserver(std::shared_ptr<Observer> observer);

protected:
    std::unique_ptr<Subject> m_ActorChanged;
};

