#include "pch.h"
#include "Player.h"
#include <GameTime.h>
#include "Grid.h"
#include <ServiceLocator.h>
#include <GameContext.h>
#include "QbertGameController.h"
#include "PlayerManager.h"

unsigned int Player::m_ID = 0;

void Player::Initialize()
{
	m_PlayerID = m_ID;
	++m_ID;
	m_TextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_TextureComponent->SetTexture("qbert.png");
	m_TextureComponent->SetSize(m_CharacterWith, m_CharacterHeight);
	m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrCols, m_NrRows);
	m_IsPlayer = true;
}

void Player::Update()
{
	//don't update when in win state
	if (GameContext::GetInstance().GetGameState() == GameState::WIN)
		return;

	if (GameContext::GetInstance().GetGameState() == GameState::COLLISION)
	{
		UpdateCollision();
		return;
	}

	if (m_State == CharacterState::jumping)
		UpdateJump();
	else if (m_State == CharacterState::fall)
		UpdateFall();
	else if (m_State == CharacterState::elevate)
		UpdateLift();
	else if (m_State == CharacterState::descend)
		UpdateDescend();
}

void Player::StopFall()
{
	m_CurrentCol = m_StartCol;
	m_CurrentRow = m_StartRow;
	m_LiftLocation = m_Grid->GetPos(m_CurrentCol, m_CurrentRow);
	--m_CurrentSpriteCol;
	ChangeLookDirection();
	m_CurrentFallSpeed = 0.f;
	m_LiftLocation.y -= m_CharacterHeight / 2;
	this->SetPosition({ m_LiftLocation, 0.f });
	m_State = CharacterState::idle;
}

void Player::StopDescend()
{
	m_CurrentCol = m_DescendLocation.x;
	m_CurrentRow = m_DescendLocation.y;
	m_LiftLocation = m_Grid->GetPos(m_CurrentCol, m_CurrentRow);
	--m_CurrentSpriteCol;
	ChangeLookDirection();
	m_CurrentFallSpeed = 0.f;
	m_LiftLocation.y -= m_CharacterHeight / 2;
	this->SetPosition({ m_LiftLocation, 0.f });
	m_State = CharacterState::idle;
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
			//check if state hasn't changed
			if (m_State == CharacterState::jumping)
			{
				--m_CurrentSpriteCol;
				m_State = CharacterState::idle;
			}
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
			//check if state hasn't changed
			if (m_State == CharacterState::jumping)
			{
				--m_CurrentSpriteCol;
				m_State = CharacterState::idle;
			}
			ChangeLookDirection();
		}
		else
			transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
	}
}

void Player::UpdateFall()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.y += m_CurrentFallSpeed;
	int x, y;
	SDL_GetWindowSize(GameContext::GetInstance().GetWindow(), &x, &y);
	if (curPos2.y > (float)y)
	{
		StopFall();
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Player::UpdateLift()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };
	curPos2 += m_LiftDirection * (m_LiftSpeed * m_LiftDistance * Time::GetInstance().GetElapsedTime());

	if (m_LiftDirection.x > 0.f)
	{
		if (curPos2.x > m_LiftLocation.x)
		{
			transformComp->SetPosition(m_LiftLocation.x, m_LiftLocation.y, 0.f);
			m_State = CharacterState::descend;
			m_LiftLocation = m_Grid->GetPos(9, 3);
		}
		else
			transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);

	}
	else if (m_LiftDirection.x < 0.f)
	{
		if (curPos2.x < m_LiftLocation.x)
		{
			transformComp->SetPosition(m_LiftLocation.x, m_LiftLocation.y, 0.f);
			m_State = CharacterState::descend;
			m_DescendLocation = m_Grid->GetTop();
			m_LiftLocation = m_Grid->GetPos(m_DescendLocation.x, m_DescendLocation.y);
			m_LiftLocation.y -= m_CharacterHeight / 2;
		}
		else
			transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
	}
}

void Player::UpdateDescend()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.y += m_CurrentFallSpeed;
	if (curPos2.y > m_LiftLocation.y)
	{
		StopDescend();
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Player::UpdateCollision()
{
	m_Timer += Time::GetInstance().GetElapsedTime();
	if (m_Timer >= m_TimeClearEnemies)
	{
		m_ActorChanged->Notify(shared_from_this(), "CLEAR_ENEMIES");
	}
	if (m_Timer >= m_TimeRestart)
	{
		SetStartLocation(m_StartCol, m_StartRow);
		m_Timer = 0.f;
		GameContext::GetInstance().Play();
		--m_CurrentSpriteCol;
		ChangeLookDirection();
		m_State = CharacterState::idle;
	}
}

void Player::SetGrid(std::shared_ptr<Grid> grid)
{
	m_Grid = grid;
}

void Player::SetStartLocation(const int& col, const int& row)
{
	m_StartCol = m_CurrentCol = m_DescendLocation.x = col;
	m_StartRow = m_CurrentRow = m_DescendLocation.y = row;

	if (m_Grid == nullptr)
		return;

	glm::vec2 pos;

	pos = m_LiftLocation = m_Grid->GetPos((short)col, (short)row);
	if (m_State != CharacterState::descend && m_State != CharacterState::fall)
	{
		m_State = CharacterState::idle;
	}
	else
		pos = m_Grid->GetPos((short)col, (short)row-2);
	pos.y -= m_CharacterHeight/2;
	this->SetPosition({ pos, 0.f });
}

void Player::DecreaseHealth()
{
	if (m_Health > 0)
	{
		--m_Health;
		if (m_Health == 0)
		{
			PlayerManager::GetInstance().SetScore(m_Score, m_PlayerID);
			PlayerManager::GetInstance().RemovePlayer(m_PlayerID);
			auto thisObject = GetGameObject();
			GetGameObject()->GetScene()->RemoveObject(std::shared_ptr<GameObject>(thisObject));
		}
	}
	m_ActorChanged->Notify(shared_from_this(), "IS_DEAD");
}

void Player::Fall()
{
	m_State = CharacterState::fall;
}

void Player::Lift()
{
	m_State = CharacterState::elevate;

	int rowOffset = 2;
	auto top = m_Grid->GetTop();
	m_LiftLocation = m_Grid->GetPos(top.x, top.y - rowOffset);
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	m_LiftDirection = m_LiftLocation - curPos2;
	m_LiftDirection = glm::normalize(m_LiftDirection);

	//distance, is how many blocks from eachother
	m_LiftDistance = m_CurrentRow - (top.y - rowOffset);
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
	m_JumpLocation.y -= m_CharacterHeight / 2;
	//get player location
	glm::vec3 curPos3 = m_GameObject->GetComponent<Transform>()->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };
	//get direction and normalize
	m_JumpDirection = m_JumpLocation - curPos2;
	m_JumpDirection = glm::normalize(m_JumpDirection);

	int random = rand() % 4 + 1;
	ServiceLocator::GetSoundSystem().Play(random, 100);
}

void Player::ChangeLookDirection(const int& horDir, const int& verDir)
{
	//if no direction is specifide, take the current col
	if (horDir == 0 || verDir == 0)
		m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrCols, m_NrRows);

	//bottomLeft
	if (horDir < 0 && verDir > 0)
		m_CurrentSpriteCol = m_BottomLeftSprite;
	//bottom right
	else if (horDir > 0 && verDir > 0)
		m_CurrentSpriteCol = m_BottomRightSprite;
	//top left
	else if (horDir < 0 && verDir < 0)
		m_CurrentSpriteCol = m_TopLeftSprite;
	//top right
	else if (horDir > 0 && verDir < 0)
		m_CurrentSpriteCol = m_TopRightSprite;

	m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrCols, m_NrRows);
}

int Player::GetIndex() const
{
	return m_Grid->GetIndex(m_CurrentCol, m_CurrentRow);
}

Shape::Rect Player::GetRect()
{
	auto transform = m_GameObject->GetComponent<Transform>();
	Shape::Rect rect;
	rect.x = transform->GetPosition().x;
	rect.y = transform->GetPosition().y;
	rect.w = (float)m_CharacterWith;
	rect.h = m_CharacterHeight;
	return rect;
}

void Player::Reset()
{
	SetStartLocation(m_StartCol, m_StartRow);
}
