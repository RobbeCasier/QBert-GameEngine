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
    void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
    void Render(const glm::vec3& position) const;
private:
    std::shared_ptr<Texture2D> m_pTexture = nullptr;
};

