#pragma once
#include <BaseComponent.h>
#include <TextureComponent.h>

#include "PlayerController.h"

enum class RBState
{
	idle,
	jump,
	fall,
	descend
};

class Grid;
class Player;
class RedBall:
	public BaseComponent,
	public std::enable_shared_from_this<RedBall>
{
public:
	explicit RedBall() = default;
	virtual ~RedBall() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void SetStartLocation(const int& col, const int& row);
	void SetGrid(std::shared_ptr<Grid> grid) { m_pGrid = grid; }
	void SetPlayers(const std::vector<std::shared_ptr<Player>>& players);

	void AddObserver(std::shared_ptr<Observer> observer);

	void Fall() { m_State = RBState::fall; }

	int GetID() { return m_ID; }
private:
	////Functions
	void UpdateIdle();
	void UpdateJump();
	void UpdateFall();
	void UpdateDescend();

	void CheckCollision();

	void ChangeTexture();

	void Jump(const int& col, const int& row);

	std::shared_ptr<TextureComponent> m_pTextureComponent = nullptr;
	std::vector<std::shared_ptr<Player>> m_Players;
	std::shared_ptr<Grid> m_pGrid = nullptr;

	std::unique_ptr<Subject> m_ActorChanged = nullptr;

	////Texture variables
	const unsigned int m_CharacterWidth = 32;
	const unsigned int m_CharacterHeight = 32;

	const unsigned int m_NrOfCols = 2;
	const unsigned int m_NrOfRows = 1;

	unsigned int m_CurrentSpriteCol = 1;
	unsigned int m_CurrentSpriteRow = 0;

	////Time Variables
	const float m_TimeTillNextJump = 0.4f;
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
	RBState m_State = RBState::descend;

	////Commands
	std::vector<std::shared_ptr<Command>> m_cmdPlayerDeaths;
};

