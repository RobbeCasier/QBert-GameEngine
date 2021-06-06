#pragma once
#include <PlayerComponent.h>

class Grid;
enum class CharacterState
{
    idle,
    jumping,
    fall,
    elevate,
    descend
};

class Player :
    public PlayerComponent
{
public:
    explicit Player() : PlayerComponent{} {};
    virtual ~Player() = default;

    virtual void Initialize() override;
    virtual void Update() override;

    void SetGrid(std::shared_ptr<Grid> grid);
    void SetStartLocation(const int& col, const int& row);

    void DecreaseHealth();
    void Fall();
    void Lift();
    void AddScore(const int& score);
    void Jump(const int& col, const int& row);

    int GetHealth() const { return m_Health; }
    int GetScore() const { return m_Score; }
    int GetIndex() const;
    Shape::Rect GetRect();

    void Reset();

protected:
    virtual void StopFall();
    virtual void StopDescend();
    void UpdateJump();
    void UpdateFall();
    void UpdateDescend();
    void UpdateCollision();
    virtual void ChangeLookDirection(const int& horDir = 0, const int& verDir = 0);
    std::shared_ptr<TextureComponent> m_TextureComponent;
    const unsigned int m_CharacterWith{ 32 };
    unsigned int m_CharacterHeight{ 32 };
    CharacterState m_State = CharacterState::descend;
    int m_CurrentSpriteCol{ 7 };
    glm::vec2 m_JumpLocation;
    glm::vec2 m_LiftLocation;
    std::shared_ptr<Grid> m_Grid = nullptr;
    unsigned int m_CurrentCol = 0;
    unsigned int m_CurrentRow = 0;
    float m_CurrentFallSpeed{ 0.f };
    const unsigned int m_NrCols{ 8 };
    const unsigned int m_NrRows{ 1 };

private:
    void UpdateLift();
    int m_Health = 3;
    int m_Score = 0;

    const int m_NrOfJumpSounds = 4;

    const float m_LiftSpeed{ 25.f };
    const float m_JumpSpeed{ 150.f };

    unsigned int m_StartCol = 0;
    unsigned int m_StartRow = 0;

    unsigned int m_BottomLeftSprite = 7;
    unsigned int m_BottomRightSprite = 5;
    unsigned int m_TopLeftSprite = 3;
    unsigned int m_TopRightSprite = 1;


    float m_Size = 0.f;
    float m_LiftDistance = 0;

    glm::vec2 m_JumpDirection;
    glm::vec2 m_LiftDirection;

    float m_TimeClearEnemies = 2.f;
    float m_TimeRestart = 2.5f;
    float m_Timer = 0.f;

    bool m_IsPlayer = false;

};

