#include "pch.h"
#include "Ugg_Wrongway.h"
#include "Grid.h"
#include <GameTime.h>
#include <GameContext.h>
#include "PlayerManager.h"

int Ugg_Wrongway::m_CurrentId = 0;

void Ugg_Wrongway::Initialize()
{
	//give an id
	m_ID = m_CurrentId;
	++m_CurrentId;

	m_pTextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_pTextureComponent->SetTexture("Ugg_Wrongway.png");
	m_pTextureComponent->SetSize(m_CharacterWidth, m_CharacterHeight);
	
	if (m_Type == Type::UGG)
	{
		m_CurrentSpriteCol = m_UggSideStart;
		m_CurrentSpriteRow = m_RowUgg;
		m_CurrentDirection = m_UggDirection;
	}
	else
	{
		m_CurrentSpriteCol = m_WrongwaySideStart;
		m_CurrentSpriteRow = m_RowWrongway;
		m_CurrentDirection = m_WrongwayDirection;
	}
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);

	//set commands
	auto players = PlayerManager::GetInstance().GetPlayers();
	for (auto& player : players)
	{
		m_cmdPlayerDeaths.push_back(std::make_shared<Death>(player));
	}
}

void Ugg_Wrongway::Update()
{
	//don't update when not in play state
	if (GameContext::GetInstance().GetGameState() != GameState::PLAY)
		return;

	switch (m_State)
	{
	case UWState::idle:
		UpdateIdle();
		break;
	case UWState::jump:
		UpdateJump();
		break;
	case UWState::fall:
		UpdateFall();
		break;
	case UWState::descend:
		UpdateDescend();
		break;
	default:
		break;
	}
	CheckCollision();
}

void Ugg_Wrongway::SetStartLocation(const int& col, const int& row)
{
	if (m_pGrid == nullptr)
		return;

	m_CurrentCol = col;
	m_CurrentRow = row;

	glm::vec2 pos;

	pos = m_FallLocation = m_pGrid->GetPos((short)col, (short)row);
	if (m_State != UWState::descend && m_State != UWState::fall)
	{
		m_State = UWState::idle;
	}
	else
		pos = m_pGrid->GetPos((short)col + (-m_CurrentDirection * 2), (short)row);
	pos.x += (-m_CurrentDirection * m_Offset * 1.5f);
	m_FallLocation.x += (-m_CurrentDirection * m_Offset * 1.5f);
	pos.y += m_CharacterHeight;
	m_FallLocation.y += m_CharacterHeight;

	m_GameObject->SetPosition(pos.x, pos.y);
}

void Ugg_Wrongway::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_ActorChanged == nullptr)
		m_ActorChanged = std::make_unique<Subject>();
	m_ActorChanged->AddObserver(observer);
}

void Ugg_Wrongway::UpdateIdle()
{
	m_Timer += Time::GetInstance().GetElapsedTime();
	if (m_Timer < m_TimeTillNextJump)
		return;
	m_Timer = 0.f;
	Jump();
}

void Ugg_Wrongway::UpdateJump()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	curPos2 += m_JumpDirection * (m_JumpSpeed * Time::GetInstance().GetElapsedTime());

	if ((m_Type == Type::UGG && curPos2.x <= m_JumpLocation.x)
		||
		(m_Type == Type::WRONGWAY && curPos2.x >= m_JumpLocation.x))
	{
		//transform
		transformComp->SetPosition(m_JumpLocation.x, m_JumpLocation.y, 0.f);
		m_pGrid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);
		if (m_State == UWState::jump)
		{
			m_CurrentSpriteCol = m_CurrentSpriteStartCol;
			m_State = UWState::idle;
			ChangeLookDirection(false);
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

void Ugg_Wrongway::UpdateFall()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.x += (m_CurrentDirection * m_CurrentFallSpeed); //they go sideways
	int x, y;
	SDL_GetWindowSize(GameContext::GetInstance().GetWindow(), &x, &y);
	if ((m_Type == Type::UGG && curPos2.x < 0.f)
		||
		(m_Type == Type::WRONGWAY && curPos2.x > (float)x))
	{
		m_ActorChanged->Notify(shared_from_this(), "KILL_UW");
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Ugg_Wrongway::UpdateDescend()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();
	glm::vec3 curPos3 = transformComp->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };

	m_CurrentFallSpeed += 9.81f * Time::GetInstance().GetElapsedTime();
	curPos2.x += (m_CurrentDirection * m_CurrentFallSpeed); //they go sideways
	int x, y;
	SDL_GetWindowSize(GameContext::GetInstance().GetWindow(), &x, &y);
	if ((m_Type == Type::UGG && curPos2.x < m_FallLocation.x)
		||
		(m_Type == Type::WRONGWAY && curPos2.x > m_FallLocation.x))
	{
		m_CurrentSpriteCol = m_CurrentSpriteStartCol;
		ChangeLookDirection(false);
		m_CurrentFallSpeed = 0.f;
		m_FallLocation.x += (-m_CurrentDirection * m_Offset * 1.5f);
		m_FallLocation.y += m_CharacterHeight;
		m_GameObject->SetPosition(m_FallLocation.x, m_FallLocation.y);
		m_State = UWState::idle;
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void Ugg_Wrongway::CheckCollision()
{
	auto players = PlayerManager::GetInstance().GetPlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i]->IsUsingLift())
			continue;

		auto playerRect = players[i]->GetRect();
		Shape::Rect rect;
		auto transform = m_GameObject->GetComponent<Transform>();
		rect.x = transform->GetPosition().x;
		rect.y = transform->GetPosition().y;
		//make it a square, so the player only has to worry about the landing
		rect.h = rect.w = (float)m_CharacterWidth;
		if (Utility::IsOverlappingRectangle(rect, playerRect))
		{
			m_cmdPlayerDeaths[i]->Execute();
		}
	}
}

void Ugg_Wrongway::Jump()
{
	m_State = UWState::jump;

	//get random jump direction
	JumpDirection jumpDir = (JumpDirection)(rand() % 2);

	//Update current grid position
	switch (jumpDir)
	{
	case JumpDirection::SIDE:
		m_CurrentCol += m_CurrentDirection * 2;
		break;
	case JumpDirection::UP:
		m_CurrentCol += m_CurrentDirection;
		break;
	default:
		break;
	}

	m_CurrentRow -= (int)jumpDir;

	//change the look direction
	ChangeLookDirection(true, jumpDir);

	//get jump location
	m_JumpLocation = m_pGrid->GetPos((short)m_CurrentCol, (short)m_CurrentRow);
	m_JumpLocation.x += (-m_CurrentDirection * m_Offset * 1.5f);
	m_JumpLocation.y += m_CharacterHeight;
	//get player location
	glm::vec3 curPos3 = m_GameObject->GetComponent<Transform>()->GetPosition();
	glm::vec2 curPos2{ curPos3.x, curPos3.y };
	//get direction and normalize
	m_JumpDirection = m_JumpLocation - curPos2;
	m_JumpDirection = glm::normalize(m_JumpDirection);
}

void Ugg_Wrongway::ChangeLookDirection(bool changeDir, JumpDirection jumpDir)
{
	if (changeDir)
	{
		switch (jumpDir)
		{
		case JumpDirection::SIDE:
			if (m_Type == Type::UGG)
				m_CurrentSpriteStartCol = m_CurrentSpriteCol = m_UggSideStart;
			else
				m_CurrentSpriteStartCol = m_CurrentSpriteCol = m_WrongwaySideStart;
			break;
		case JumpDirection::UP:
			if (m_Type == Type::UGG)
				m_CurrentSpriteStartCol = m_CurrentSpriteCol = m_UggUpStart;
			else
				m_CurrentSpriteStartCol = m_CurrentSpriteCol = m_WrongwayUpStart;
			break;
		default:
			break;
		}
		//extra offset
		++m_CurrentSpriteCol;
	}

	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);
}
