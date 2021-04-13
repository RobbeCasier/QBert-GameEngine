#include "pch.h"
#include "Player.h"
#include <GameTime.h>
#include "Grid.h"

void Player::Initialize()
{
	m_TextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_TextureComponent->SetTexture("qbert.png", m_CharacterWith);
	m_TextureComponent->SetSource(6, 0, m_NrCols, m_NrRows);
}

void Player::Update()
{
	if (m_State == CharacterState::jumping)
		UpdateJump();
}

void Player::UpdateJump()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	curPos2 += m_JumpDirection * (m_JumpSpeed * Time::GetInstance().GetElapsedTime());

	if (m_JumpDirection.x > 0.f)
	{
		if (curPos2.x > m_JumpLocation.x)
		{
			transformComp->SetPosition(m_JumpLocation.x, m_JumpLocation.y, 0.f);
			m_Grid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);
			--m_CurrentSpriteCol;
			m_State = CharacterState::idle;
			ChangeLookDirection();
		}
		else
			transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);

	}
	else if (m_JumpDirection.x < 0.f)
	{
		if (curPos2.x < m_JumpLocation.x)
		{
			transformComp->SetPosition(m_JumpLocation.x, m_JumpLocation.y, 0.f);
			m_Grid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);
			--m_CurrentSpriteCol;
			m_State = CharacterState::idle;
			ChangeLookDirection();
		}
		else
			transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
	}
}

void Player::SetGrid(std::shared_ptr<Grid> grid)
{
	m_Grid = grid;
}

void Player::SetGridPosition(const int& col, const int& row)
{
	m_State = CharacterState::idle;
	m_CurrentCol = col;
	m_CurrentRow = row;

	if (m_Grid == nullptr)
		return;

	glm::vec2 pos = m_Grid->GetPos((short)col, (short)row);
	this->SetPosition({ pos, 0.f });
}

void Player::DecreaseHealth()
{
	if (m_Health > 0)
		--m_Health;
	m_ActorChanged->Notify(shared_from_this(), "IS_DEAD");
}

void Player::AddScore(const int& score)
{
	m_Score += score;
	m_ActorChanged->Notify(shared_from_this(), "SCORE_ADDED");
}

void Player::Jump(const int& col, const int& row)
{
	if (m_State == CharacterState::jumping)
		return;

	m_State = CharacterState::jumping;

	//update current grid position
	m_CurrentCol += col;
	m_CurrentRow += row;

	//change the look direction if needed
	ChangeLookDirection(col, row);

	//get jump location
	m_JumpLocation = m_Grid->GetPos((short)m_CurrentCol, (short)m_CurrentRow);
	//get player location
	glm::vec3 curPos3 = m_GameObject->GetComponent<Transform>()->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };
	//get direction and normalize
	m_JumpDirection = m_JumpLocation - curPos2;
	m_JumpDirection = glm::normalize(m_JumpDirection);
}

void Player::ChangeLookDirection(const int& horDir, const int& verDir)
{
	//if no direction is specifide, take the current col
	if (horDir == 0 || verDir == 0)
		m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrCols, m_NrRows);

	//bottomLeft
	if (horDir < 0 && verDir > 0)
		m_CurrentSpriteCol = 7;
	//bottom right
	else if (horDir > 0 && verDir > 0)
		m_CurrentSpriteCol = 5;
	//top left
	else if (horDir < 0 && verDir < 0)
		m_CurrentSpriteCol = 3;
	//top right
	else if (horDir > 0 && verDir < 0)
		m_CurrentSpriteCol = 1;

	m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrCols, m_NrRows);
}
