#include "pch.h"
#include "Slick_Sam.h"
#include "Grid.h"
#include <GameTime.h>
#include <GameContext.h>

int Slick_Sam::m_CurrentId = 0;
Slick_Sam::~Slick_Sam()
{
	--m_CurrentId;
}

void Slick_Sam::Initialize()
{
	m_ID = m_CurrentId;
	++m_CurrentId;

	m_pTextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_pTextureComponent->SetTexture("Slick_Slam.png");
	m_pTextureComponent->SetSize(m_CharacterWidth, m_CharacterHeight);

	m_Type = (SSType)(rand() % (int)SSType::count);

	m_CurrentSpriteCol = 0;
	if (m_Type == SSType::SLICK)
		m_CurrentSpriteRow = m_RowSlick;
	else
		m_CurrentSpriteRow = m_RowSam;

	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);

}

void Slick_Sam::Update()
{
	switch (m_State)
	{
	case SSState::idle:
		UpdateIdle();
		break;
	case SSState::jump:
		UpdateJump();
		break;
	case SSState::fall:
		UpdateFall();
		break;
	case SSState::descend:
		UpdateDescend();
		break;
	default:
		break;
	}

	CheckCollision();
}

void Slick_Sam::SetPlayer(std::shared_ptr<Player> player)
{
	m_pPlayer = player;
	m_cmdCatch = std::make_shared<Catch>(m_pPlayer);
}

void Slick_Sam::SetStartLocation(const int& col, const int& row)
{
	
	if (m_pGrid == nullptr)
		return;

	m_CurrentCol = col;
	m_CurrentRow = row;

	glm::vec2 pos;

	//set the start position where the character start falling
	pos = m_FallLocation = m_pGrid->GetPos((short)col, (short)row);
	if (m_State != SSState::descend && m_State != SSState::fall)
	{
		m_State = SSState::idle;
	}
	else
		pos = m_pGrid->GetPos((short)col, (short)row - 2);
	pos.y -= m_CharacterHeight;
	m_FallLocation.y -= m_CharacterHeight;

	m_GameObject->SetPosition(pos.x, pos.y);

}

void Slick_Sam::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_ActorChanged == nullptr)
		m_ActorChanged = std::make_unique<Subject>();
	m_ActorChanged->AddObserver(observer);
}

void Slick_Sam::UpdateIdle()
{
	m_Timer += Time::GetInstance().GetElapsedTime();
	if (m_Timer < m_TimeTillNextJump)
		return;
	m_Timer = 0.f;

	//random direction
	int dir = 0;
	int randomNr = rand() % 2;
	if (randomNr == 0)
		dir = -1;
	else
		dir = 1;

	++m_CurrentSpriteCol;
	Jump(dir, 1);
}

void Slick_Sam::UpdateJump()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	curPos2 += m_JumpDirection * (m_JumpSpeed * Time::GetInstance().GetElapsedTime());

	if (curPos2.y >= m_JumpLocation.y)
	{
		transformComp->SetPosition(m_JumpLocation.x, m_JumpLocation.y, 0.f);

		//check the grid for the appropriat action
		m_pGrid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);

		if (m_State == SSState::jump)
		{
			m_CurrentSpriteCol = m_CurrentSpriteStartCol;
			m_State = SSState::idle;
			ChangeLookDirection();
		}
	}
	else
	{
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);

		m_AnimationTimer += Time::GetInstance().GetElapsedTime();
		//update animation
		if (m_AnimationTimer >= m_TimeTillNextFrame)
		{
			++m_CurrentJumpAnimationFrame;
			int spriteOffset = m_CurrentSpriteStartCol + 1;
			m_CurrentSpriteCol = (m_CurrentJumpAnimationFrame % m_MaxJumpAnimationFrames) + spriteOffset;
			m_AnimationTimer -= m_TimeTillNextFrame;
			ChangeLookDirection(false);
		}
	}
}

void Slick_Sam::UpdateFall()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	//calc next position
	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.y += m_CurrentFallSpeed;
	int x, y;
	SDL_GetWindowSize(GameContext::GetInstance().GetWindow(), &x, &y);

	//check if crossed border
	if (curPos2.y > (float)y)
		m_ActorChanged->Notify(shared_from_this(), "KILL_SS"); //No Germans
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Slick_Sam::UpdateDescend()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	//calc next position
	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.y += m_CurrentFallSpeed;
	int x, y;
	SDL_GetWindowSize(GameContext::GetInstance().GetWindow(), &x, &y);

	//check if out of the screen
	if (curPos2.y > m_FallLocation.y)
	{
		m_CurrentSpriteCol = m_CurrentSpriteStartCol;
		ChangeLookDirection();
		m_CurrentFallSpeed = 0.f;
		m_FallLocation.y += m_Offset;
		m_GameObject->SetPosition(m_FallLocation.x, m_FallLocation.y);

		//check the grid for the appropriat action
		m_pGrid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);
		m_State = SSState::idle;
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Slick_Sam::CheckCollision()
{
	auto playerRect = m_pPlayer->GetRect();
	Shape::Rect rect;
	auto transform = m_GameObject->GetComponent<Transform>();
	rect.x = transform->GetPosition().x;
	rect.y = transform->GetPosition().y;
	//make it a square, so the player only has to worry about the landing
	rect.h = rect.w = m_CharacterWidth;
	if (Utility::IsOverlappingRectangle(rect, playerRect))
	{
		m_cmdCatch->Execute();
		m_ActorChanged->Notify(shared_from_this(), "KILL_SS"); //No Germans
	}
}

void Slick_Sam::Jump(const int& col, const int& row)
{
	if (m_State == SSState::jump)
		return;

	m_State = SSState::jump;

	//update current grid position
	m_CurrentCol += col;
	m_CurrentRow += row;

	//change the look direction if needed
	ChangeLookDirection(col, row);

	//get jump location
	m_JumpLocation = m_pGrid->GetPos((short)m_CurrentCol, (short)m_CurrentRow);
	m_JumpLocation.y -= m_Offset;
	//get player location
	glm::vec3 curPos3 = m_GameObject->GetComponent<Transform>()->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };
	//get direction and normalize
	m_JumpDirection = m_JumpLocation - curPos2;
	m_JumpDirection = glm::normalize(m_JumpDirection);
}

void Slick_Sam::ChangeLookDirection(const int& horDir, const int& verDir)
{
	if (horDir != 0 && verDir != 0)
	{
		//bottomLeft
		if (horDir < 0 && verDir > 0)
			m_CurrentSpriteCol = m_LeftStart;
		//bottom right
		else if (horDir > 0 && verDir > 0)
			m_CurrentSpriteCol = m_RightStart;
	}
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);
}
