#pragma once
#include <BaseComponent.h>
#include <TextureComponent.h>
#include <Subject.h>

#include "PlayerController.h"

enum class SSType
{
    SLICK,
    SAM,
    count = 2
};

enum class SSState
{
    idle,
    jump,
    fall,
    descend
};

class Grid;
class Player;

class Slick_Sam :
    public BaseComponent, public std::enable_shared_from_this<Slick_Sam>
{
public:
    explicit Slick_Sam() = default;
    virtual ~Slick_Sam();

    virtual void Initialize() override;
    virtual void Update() override;

    void SetPlayer(std::shared_ptr<Player> player);
    void SetGrid(std::shared_ptr<Grid> grid) { m_pGrid = grid; }
    void SetStartLocation(const int& col, const int& row);

    void AddObserver(std::shared_ptr<Observer> observer);

    void Fall() { m_State = SSState::fall; }

    int GetID() { return m_ID; }

private:
    ////Functions
    void UpdateIdle();
    void UpdateJump();
    void UpdateFall();
    void UpdateDescend();

    void CheckCollision();

    void Jump(const int& col, const int& row);

    void ChangeLookDirection(const int& horDir = 0, const int& verDir = 0);

    ////Pointers
    std::shared_ptr<TextureComponent> m_pTextureComponent = nullptr;
    std::shared_ptr<Player> m_pPlayer = nullptr;
    std::shared_ptr<Grid> m_pGrid = nullptr;

    std::unique_ptr<Subject> m_ActorChanged = nullptr;

    ////Texture variables
    const unsigned int m_CharacterWidth = 32.f;
    const unsigned int m_CharacterHeight = 32.f;

    //starter column depending on character
    const unsigned int m_LeftStart = 0;
    const unsigned int m_RightStart = 4;

    //row depending on character
    const unsigned int m_RowSlick = 0;
    const unsigned int m_RowSam = 1;

    const unsigned int m_NrOfCols = 8;
    const unsigned int m_NrOfRows = 2;

    unsigned int m_CurrentSpriteCol;
    unsigned int m_CurrentSpriteRow;
    unsigned int m_CurrentSpriteStartCol;

    ////Time Variables
    const float m_TimeTillNextJump = 0.2f;
    float m_Timer = 0.f;

    ////Animation
    const unsigned int m_MaxJumpAnimationFrames = 3;
    unsigned int m_CurrentJumpAnimationFrame = 0;
    const float m_TimeTillNextFrame = 0.1f;
    float m_AnimationTimer = 0.f;

    //Extra
    unsigned int m_CurrentCol = 0;
    unsigned int m_CurrentRow = 0;

    float m_Offset = 16.f;

    float m_CurrentFallSpeed = 0.f;

    const float m_JumpSpeed = 140.f;

    glm::vec2 m_JumpLocation;
    glm::vec2 m_JumpDirection;

    glm::vec2 m_FallLocation;

    static int m_CurrentId;
    int m_ID;

    ////Enum variables
    SSType m_Type;
    SSState m_State = SSState::descend;

    ////Commands
    std::shared_ptr<Command> m_cmdCatch;
};

