#include "pch.h"
#include "Coily.h"
#include "Grid.h"
#include <GameTime.h>

void Coily::Initialize()
{
	m_TextureComponent = m_GameObject->AddComponent<TextureComponent>();
	m_TextureComponent->SetTexture("CoilyEgg.png");
	m_TextureComponent->SetSize(m_CharacterWith, m_CharacterWith);
	m_CurrentSpriteCol = 1;
	m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrColsEgg, m_NrRowsEgg);
}

void Coily::Update()
{
	Player::Update();

	if (m_State == CharacterState::idle)
	{
		m_Timer += Time::GetInstance().GetElapsedTime();
		if (m_Grid->GetBottomLine() == m_CurrentRow && m_CState != CoilyState::snake)
		{
			if (m_Timer >= m_TimeTillHatch)
			{
				m_CState = CoilyState::snake;
				m_CharacterHeight = 2 * m_CharacterWith;
				m_TextureComponent->SetSize(m_CharacterWith, m_CharacterHeight);
				m_TextureComponent->SetTexture("Coily.png");
				m_TextureComponent->SetSource(6, 0, m_NrCols, m_NrRows);
				auto pos = m_GameObject->GetComponent<Transform>()->GetPosition();
				pos.y -= m_CharacterHeight / 2;
				this->SetPosition(pos);
				m_Timer = m_Timer - m_TimeTillHatch;
			}
		}
		else if (m_Timer >= m_TimeTilNextJump)
		{
			m_Timer = m_Timer - m_TimeTilNextJump;
			if (m_CState == CoilyState::egg)
				GetRandomJumpLocation();
			else
			{
				if (m_pGraph == nullptr)
					m_pGraph = m_Grid->GetGraph();

				int currentIndex = GetIndex();
				int endIndex = m_pPlayer->GetIndex();
				m_pGraph->FindPath(currentIndex, endIndex);
				int nextNode = m_pGraph->GetNextNode();
				glm::vec2 columnRow = m_Grid->GetColumnRow(nextNode);

				int col = columnRow[0] - m_CurrentCol;
				int row = columnRow[1] - m_CurrentRow;

				Jump(col, row);

			}

		}
	}

	CheckCollision();
}

void Coily::AddPlayer(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_cmdPlayerDeath = std::make_shared<Death>(m_pPlayer);
}

void Coily::StopFall()
{
	m_ActorChanged->Notify(shared_from_this(), "KILL_COILY");
}

void Coily::StopDescend()
{
	--m_CurrentSpriteCol;
	ChangeLookDirection();
	m_CurrentFallSpeed = 0.f;
	m_LiftLocation.y -= m_CharacterHeight / 2;
	this->SetPosition({ m_LiftLocation, 0.f });
	m_State = CharacterState::idle;
}

void Coily::ChangeLookDirection(const int& horDir, const int& verDir)
{
	if (m_CState == CoilyState::egg)
		m_TextureComponent->SetSource(m_CurrentSpriteCol, 0, m_NrColsEgg, m_NrRowsEgg);
	else
		Player::ChangeLookDirection(horDir, verDir);
}

void Coily::GetRandomJumpLocation()
{
	//get random direction
	int dir = 0;
	int randomNr = rand() % 2;
	if (randomNr == 0)
		dir = -1;
	else
		dir = 1;

	++m_CurrentSpriteCol;
	Jump(dir, 1);
}

void Coily::CheckCollision()
{
	auto playerRect = m_pPlayer->GetRect();
	Shape::Rect rect;
	auto transform = m_GameObject->GetComponent<Transform>();
	rect.x = transform->GetPosition().x;
	rect.y = transform->GetPosition().y;
	//make it a square, so the player only has to worry about the landing
	rect.h = rect.w = m_CharacterWith;
	if (Utility::IsOverlappingRectangle(rect, playerRect))
	{
		m_cmdPlayerDeath->Execute();
	}
}
