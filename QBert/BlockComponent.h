#pragma once
#include <BaseComponent.h>
#include <TextureComponent.h>
#include <PlayerComponent.h>
#include <Commands.h>
#include "LevelReader.h"

class Grid;

enum class ColorState
{
    START,
    INTERMEDIAT,
    FINAL
};
class BlockComponent :
    public BaseComponent
{
public:
    explicit BlockComponent() = default;
    virtual ~BlockComponent() = default;

    virtual void Initialize() override;
    virtual void Update() override {};
    virtual void Render(const glm::vec3& position) override;

    void SetGameType(GameType type);
    void SetIsSide(bool isSide);
    void SetTexture(SideColor color, int width = 0, int height = 0);
    void SetColorOrder(std::vector<int> colorOrder);
    void SetTextureDestination(glm::vec3 dst);
    ColorState ChangeColor(bool reset = false);
    bool IsRightColor() const;
private:
    glm::vec3 m_TextureDst;
    std::shared_ptr<TextureComponent> m_TextureComponent = nullptr;
    SideColor m_SideColor;
    GameType m_Type;
    std::vector<int> m_ColorOrder;

    int m_CurrentColor = 0;
    bool m_isSide = false;

    bool m_HasReachedFinalColorOnce = false;
};

