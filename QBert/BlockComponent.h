#pragma once
#include <BaseComponent.h>
#include <TextureComponent.h>
#include <PlayerComponent.h>
#include <Commands.h>

class Grid;
enum class SideColor
{
    aqua,
    orange,
    grey,
    greyBlue,
    black,
    yellowOrange,
    yellowBlue,
    green,
    blackBlack
};
class BlockComponent :
    public BaseComponent
{
public:
    explicit BlockComponent() = default;
    virtual ~BlockComponent() = default;

    virtual void Initialize() override;
    virtual void Update() override {};

    void AddPlayerCommand(std::shared_ptr<PlayerComponent> player);
    void AddGridCommand(std::shared_ptr<Grid> player);

    void SetIsSide(bool isSide);
    void SetTexture(SideColor color, int width = 0, int height = 0);
    void SetColorOrder(std::vector<int> colorOrder);
    void SetTextureDestination(glm::vec3 dst);
    void ChangeColor();
    bool IsRightColor() const;
private:
    glm::vec3 m_TextureDst;
    std::shared_ptr<TextureComponent> m_TextureComponent = nullptr;
    SideColor m_SideColor;
    std::vector<int> m_ColorOrder;

    int m_CurrentColor = 0;
    bool m_isSide = false;

    std::unique_ptr<Command> m_BlockChangePlayer;
    std::unique_ptr<Command> m_BlockChangeGrid;
};
