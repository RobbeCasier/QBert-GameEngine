#include "pch.h"
#include "GreenBall.h"
#include "Grid.h"
#include "GameTime.h"
#include "GameContext.h"

int GreenBall::m_CurrentId = 0;

void GreenBall::Initialize()
{
	m_ID = m_CurrentId;
	++m_CurrentId;
	//setup texture
	m_pTextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_pTextureComponent->SetTexture("Red_Ball.png");
	m_pTextureComponent->SetSize(m_CharacterWidth, m_CharacterHeight);

	//setup texture source
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);
}

void GreenBall::Update()
{
	//don't update when in win state
	if (GameContext::GetInstance().GetGameState() != GameState::PLAY)
		return;

	switch (m_State)
	{
	case GBState::idle:
		UpdateIdle();
		break;
	case GBState::jump:
		UpdateJump();
		break;
	case GBState::fall:
		UpdateFall();
		break;
	case GBState::descend:
		UpdateDescend();
		break;
	default:
		break;
	}

	CheckCollision();
}

void GreenBall::SetStartLocation(const int& col, const int& row)
{
	if (m_pGrid == nullptr)
		return;

	m_CurrentCol = col;
	m_CurrentRow = row;

	glm::vec2 pos;

	//set the start position where the character start falling
	pos = m_FallLocation = m_pGrid->GetPos((short)col, (short)row);
	if (m_State != GBState::descend && m_State != GBState::fall)
	{
		m_State = GBState::idle;
	}
	else
		pos = m_pGrid->GetPos((short)col, (short)row - 2);
	pos.y -= m_CharacterHeight;
	m_FallLocation.y -= m_CharacterHeight;

	m_GameObject->SetPosition(pos.x, pos.y);
}

void GreenBall::SetPlayers(const std::vector<std::shared_ptr<Player>>& players)
{
	m_Players = players;
	for (auto player : m_Players)
	{
		m_cmdPlayerDeaths.push_back(std::make_shared<Death>(player));
	}
}

void GreenBall::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_ActorChanged == nullptr)
		m_ActorChanged = std::make_unique<Subject>();
	m_ActorChanged->AddObserver(observer);
}

void GreenBall::UpdateIdle()
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

void GreenBall::UpdateJump()
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

		if (m_State == GBState::jump)
		{
			--m_CurrentSpriteCol;
			ChangeTexture();
			m_State = GBState::idle;
		}
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void GreenBall::UpdateFall()
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
		m_ActorChanged->Notify(shared_from_this(), "KILL_GB");
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void GreenBall::UpdateDescend()
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
		--m_CurrentSpriteCol;
		ChangeTexture();
		m_CurrentFallSpeed = 0.f;
		m_FallLocation.y += m_Offset;
		m_GameObject->SetPosition(m_FallLocation.x, m_FallLocation.y);

		//check the grid for the appropriat action
		m_pGrid->CheckGrid(shared_from_this(), m_CurrentCol, m_CurrentRow);
		m_State = GBState::idle;
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void GreenBall::CheckCollision()
{
	for (int i = 0; i < m_Players.size(); ++i)
	{
		auto playerRect = m_Players[i]->GetRect();
		playerRect /= 2.0f;
		Shape::Rect rect;
		auto transform = m_GameObject->GetComponent<Transform>();
		rect.h = rect.w = (float)m_CharacterWidth;
		rect.x = transform->GetPosition().x;
		rect.y = transform->GetPosition().y;
		rect /= 2.0f;
		if (Utility::IsOverlappingRectangle(rect, playerRect))
		{
			GameContext::GetInstance().GreenBallMode();
			return;
		}
	}
}

void GreenBall::ChangeTexture()
{
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);
}

void GreenBall::Jump(const int& col, const int& row)
{
	if (m_State == GBState::jump)
		return;

	m_State = GBState::jump;

	ChangeTexture();

	//update current grid position
	m_CurrentCol += col;
	m_CurrentRow += row;

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
