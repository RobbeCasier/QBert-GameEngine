#pragma once
#include <BaseComponent.h>
class InGameMenuUI :
    public BaseComponent
{
public:
    explicit InGameMenuUI() = default;
    virtual ~InGameMenuUI() = default;

    virtual void Initialize() override {};
    virtual void Update() override {};
    virtual void Render(const glm::vec3& position);
};

