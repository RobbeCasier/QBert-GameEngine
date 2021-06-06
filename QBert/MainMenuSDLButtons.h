#pragma once
#include <BaseComponent.h>
class MainMenuSDLButtons :
    public BaseComponent
{
public:
    explicit MainMenuSDLButtons() = default;
    virtual ~MainMenuSDLButtons() = default;

    virtual void Initialize() override {};
    virtual void Update() override {};
    virtual void Render(const glm::vec3& position) override;
};

