#include "pch.h"
#include "RedBall.h"
#include "Grid.h"
#include "GameTime.h"
#include "GameContext.h"
#include "PlayerManager.h"

int RedBall::m_CurrentId = 0;

void RedBall::Initialize()
{
	m_ID = m_CurrentId;
	++m_CurrentId;
	//setup texture
	m_pTextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_pTextureComponent->SetTexture("Red_Ball.png");
	m_pTextureComponent->SetSize(m_CharacterWidth, m_CharacterHeight);

	//setup texture source
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);

	auto players = PlayerManager::GetInstance().GetPlayers();
	for (auto& player : players)
	{
		m_cmdPlayerDeaths.push_back(std::make_shared<Death>(player));
	}
}

void RedBall::Update()
{
	//don't update when in win state
	if (GameContext::GetInstance().GetGameState() != GameState::PLAY)
		return;

	switch (m_State)
	{
	case RBState::idle:
		UpdateIdle();
		break;
	case RBState::jump:
		UpdateJump();
		break;
	case RBState::fall:
		UpdateFall();
		break;
	case RBState::descend:
		UpdateDescend();
		break;
	default:
		break;
	}

	CheckCollision();
}

void RedBall::SetStartLocation(const int& col, const int& row)
{
	if (m_pGrid == nullptr)
		return;

	m_CurrentCol = col;
	m_CurrentRow = row;

	glm::vec2 pos;

	//set the start position where the character start falling
	pos = m_FallLocation = m_pGrid->GetPos((short)col, (short)row);
	if (m_State != RBState::descend && m_State != RBState::fall)
	{
		m_State = RBState::idle;
	}
	else
		pos = m_pGrid->GetPos((short)col, (short)row - 2);
	pos.y -= m_CharacterHeight;
	m_FallLocation.y -= m_CharacterHeight;

	m_GameObject->SetPosition(pos.x, pos.y);
}

void RedBall::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_ActorChanged == nullptr)
		m_ActorChanged = std::make_unique<Subject>();
	m_ActorChanged->AddObserver(observer);
}

void RedBall::UpdateIdle()
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

void RedBall::UpdateJump()
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

		if (m_State == RBState::jump)
		{
			--m_CurrentSpriteCol;
			ChangeTexture();
			m_State = RBState::idle;
		}
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void RedBall::UpdateFall()
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
		m_ActorChanged->Notify(shared_from_this(), "KILL_RB");
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void RedBall::UpdateDescend()
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
		m_State = RBState::idle;
	}
	else
		transformComp->SetPosition(curPos2.x, curPos2.y, 0.f);
}

void RedBall::CheckCollision()
{
	auto players = PlayerManager::GetInstance().GetPlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i]->IsUsingLift())
			continue;
		auto playerRect = players[i]->GetRect();
		playerRect /= 1.5f;
		Shape::Rect rect;
		auto transform = m_GameObject->GetComponent<Transform>();
		rect.h = rect.w = (float)m_CharacterWidth;
		rect.x = transform->GetPosition().x;
		rect.y = transform->GetPosition().y;
		rect /= 1.5f;
		if (Utility::IsOverlappingRectangle(rect, playerRect))
		{
			ServiceLocator::GetSoundSystem().Play(10, 100);
			m_cmdPlayerDeaths[i]->Execute();
			return;
		}
	}
}

void RedBall::ChangeTexture()
{
	m_pTextureComponent->SetSource(m_CurrentSpriteCol, m_CurrentSpriteRow, m_NrOfCols, m_NrOfRows);
}

void RedBall::Jump(const int& col, const int& row)
{
	if (m_State == RBState::jump)
		return;

	m_State = RBState::jump;

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
