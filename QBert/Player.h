#pragma once
#include <PlayerComponent.h>


class Grid;
enum class CharacterState
{
    idle,
    jumping
};

class Player :
    public PlayerComponent
{
public:
    explicit Player() : PlayerComponent{} {};
    virtual ~Player() = default;

    virtual void Initialize() override;
    virtual void Update() override;
    void UpdateJump();

    void SetGrid(std::shared_ptr<Grid> grid);
    void SetGridPosition(const int& col, const int& row);

    void DecreaseHealth();
    void AddScore(const int& score);
    void Jump(const int& col, const int& row);

    int GetHealth() const { return m_Health; }
    int GetScore() const { return m_Score; }
private:
    void ChangeLookDirection(const int& horDir = 0, const int& verDir = 0);
    int m_Health = 3;
    int m_Score = 0;

    const float m_JumpSpeed{ 200.f };
    unsigned int m_CurrentCol = 0;
    unsigned int m_CurrentRow = 0;
    const unsigned int m_NrCols{ 8 };
    const unsigned int m_NrRows{ 1 };
    const unsigned int m_CharacterWith{ 32 };
    std::shared_ptr<TextureComponent> m_TextureComponent;

    int m_CurrentSpriteCol{ 6 };

    glm::vec2 m_JumpLocation;
    glm::vec2 m_JumpDirection;

    CharacterState m_State = CharacterState::idle;

    std::shared_ptr<Grid> m_Grid = nullptr;

};

