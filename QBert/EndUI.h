#pragma once
#include <BaseComponent.h>

class EndUI :
    public BaseComponent
{
public:
    explicit EndUI() = default;
    virtual ~EndUI() = default;

    virtual void Initialize() override {};
    virtual void Update() override {};
    virtual void Render(const glm::vec3& position);
};

