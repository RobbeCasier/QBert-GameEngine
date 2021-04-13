#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

class RenderComponent :
    public BaseComponent
{
public:
    explicit RenderComponent() = default;
    virtual ~RenderComponent() = default;

    virtual void Initialize() override {};
    virtual void Update() override {};
    void Render(const Texture2D& texture, const glm::vec3& position) const;
    void Render(const Texture2D& texture, const glm::vec3& position, float dstWidth, float dstHeight, const glm::vec2& src, float width, float height) const;
};

