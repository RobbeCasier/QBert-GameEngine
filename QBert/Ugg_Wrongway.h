#pragma once
#include <BaseComponent.h>
#include <TextureComponent.h>
#include <Subject.h>

#include "PlayerController.h"

enum class Type
{
    UGG,
    WRONGWAY,
    count = 2
};

enum class UWState
{
    idle,
    jump,
    fall,
    descend
};

enum class JumpDirection
{
    SIDE,
    UP
};


class Grid;
class Player;

class Ugg_Wrongway :
    public BaseComponent, public std::enable_shared_from_this<Ugg_Wrongway>
{
public:
    explicit Ugg_Wrongway(Type type) : m_Type(type) {};
    virtual ~Ugg_Wrongway() = default;

    virtual void Initialize() override;
    virtual void Update() override;

    void SetPlayers(const std::vector<std::shared_ptr<Player>>& player);
    void SetGrid(std::shared_ptr<Grid> grid) { m_pGrid = grid; }
    void SetStartLocation(const int& col, const int& row);

    void AddObserver(std::shared_ptr<Observer> observer);

    void Fall() { m_State = UWState::fall; }

    int GetID() { return m_ID; }
private:
    ////Functions
    void UpdateIdle();
    void UpdateJump();
    void UpdateFall();
    void UpdateDescend();

    void CheckCollision();

    void Jump();

    void ChangeLookDirection(bool changeDir, JumpDirection jumpDir = JumpDirection::SIDE);

    ////Pointers
    std::shared_ptr<TextureComponent> m_pTextureComponent = nullptr;
    std::vector<std::shared_ptr<Player>> m_Players;
    std::shared_ptr<Grid> m_pGrid = nullptr;

    std::unique_ptr<Subject> m_ActorChanged = nullptr;

    ////Texture variables
    const unsigned int m_CharacterWidth = 32;
    const unsigned int m_CharacterHeight = 32;

    //starter column depending on character
    const unsigned int m_UggSideStart = 4;
    const unsigned int m_UggUpStart = 0;
    const unsigned int m_WrongwaySideStart = 0;
    const unsigned int m_WrongwayUpStart = 4;

    //row depending on character
    const unsigned int m_RowUgg = 0;
    const unsigned int m_RowWrongway = 1;

    const unsigned int m_NrOfCols = 8;
    const unsigned int m_NrOfRows = 2;

    unsigned int m_CurrentSpriteCol;
    unsigned int m_CurrentSpriteRow;
    unsigned int m_CurrentSpriteStartCol;

    ////Time variables
    const float m_TimeTillNextJump = 0.25f;
    float m_Timer = 0.f;

    ////Jump Frames
    const unsigned int m_MaxJumpAnimationFrames = 3;
    unsigned int m_CurrentJumpAnimationFrame = 0;
    const float m_TimeTillNextFrame = 0.1f;
    float m_AnimationTimer = 0.f;

    ////Extra
    const int m_UggDirection = -1;
    const int m_WrongwayDirection = 1;
    int m_CurrentDirection;

    unsigned int m_CurrentCol = 0;
    unsigned int m_CurrentRow = 0;

    float m_Offset = 16.f;

    float m_CurrentFallSpeed = 0.f;
    const float m_JumpSpeed = 100.f;

    glm::vec2 m_JumpLocation;
    glm::vec2 m_JumpDirection;

    glm::vec2 m_FallLocation;

    ////Enum variables
    Type m_Type;
    UWState m_State = UWState::descend;

    ////Commands
    std::vector<std::shared_ptr<Command>> m_cmdPlayerDeaths;

    static int m_CurrentId;
    int m_ID = 0;
};

