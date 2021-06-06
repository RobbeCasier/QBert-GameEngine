#include "pch.h"
#include "Coily.h"
#include "Grid.h"
#include <GameTime.h>
#include <GameContext.h>
#include <InputManager.h>
#include "EnemyController.h"

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
	//don't update when in win state
	if (GameContext::GetInstance().GetGameState() != GameState::PLAY)
		return;

	Player::Update();

	if (m_State == CharacterState::idle)
	{
		//time to stay on the bottom line
		m_Timer += Time::GetInstance().GetElapsedTime();
		if (m_Grid->GetBottomLine() == m_CurrentRow && m_CState != CoilyState::snake)
		{
			//if long enough hatch
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

				if (m_IsPossesed)
				{
					auto& input = InputManager::GetInstance();
					//controller input player 2
					std::shared_ptr<Coily> myShare = std::dynamic_pointer_cast<Coily>(shared_from_this());
					input.AddInput(ControllerButton::ButtonUP, std::make_unique<CoilyJumpTopRight>(myShare), 1);
					input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<CoilyJumpBottomRight>(myShare), 1);
					input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<CoilyJumpBottomLeft>(myShare), 1);
					input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<CoilyJumpTopLeft>(myShare), 1);

					//keyboard input player 2
					input.AddInput(KeyboardKeys::Up, std::make_unique<CoilyJumpTopRight>(myShare));
					input.AddInput(KeyboardKeys::Right, std::make_unique<CoilyJumpBottomRight>(myShare));
					input.AddInput(KeyboardKeys::Down, std::make_unique<CoilyJumpBottomLeft>(myShare));
					input.AddInput(KeyboardKeys::Left, std::make_unique<CoilyJumpTopLeft>(myShare));
				}
			}
		}
		//time for next jump
		else if (m_Timer >= m_TimeTilNextJump)
		{
			m_Timer = m_Timer - m_TimeTilNextJump;
			if (m_CState == CoilyState::egg)
				GetRandomJumpLocation();
			else if (!m_IsPossesed)
			{
				if (m_pGraph == nullptr)
					m_pGraph = m_Grid->GetGraph();

				int currentIndex = GetIndex();
				std::vector<int> endIndices;
				for (auto player : m_Players)
					endIndices.push_back(player->GetIndex());

				m_pGraph->FindPath(currentIndex, endIndices);
				int nextNode = m_pGraph->GetNextNode();
				glm::vec2 columnRow = m_Grid->GetColumnRow(nextNode);

				int col = columnRow[0] - m_CurrentCol;
				int row = columnRow[1] - m_CurrentRow;

				//check if coily doesn't jump to start position
				if (col != 0 && row != 0)
					Jump(col, row);

			}

		}
	}

	CheckCollision();
}

void Coily::AddPlayers(std::vector<std::shared_ptr<Player>> players)
{
	m_Players = players;
	for (auto player : m_Players)
	{
		m_cmdPlayerDeaths.push_back(std::make_shared<Death>(player));
	}
}

const int& Coily::GetPlayer()
{
	if (m_Players.size() == 1)
		return 0;
	return m_pGraph->GetRouteIndex();
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
	for (int i = 0; i < m_Players.size(); ++i)
	{
		auto playerRect = m_Players[i]->GetRect();
		playerRect /= 2.0f;
		Shape::Rect rect;
		auto transform = m_GameObject->GetComponent<Transform>();
		rect.h = rect.w = (float)m_CharacterWith;
		rect.x = transform->GetPosition().x;
		rect.y = transform->GetPosition().y;
		rect /= 2.0f;
		//make it a square, so the player only has to worry about the landing
		if (Utility::IsOverlappingRectangle(rect, playerRect))
		{
			m_cmdPlayerDeaths[i]->Execute();
			m_ActorChanged->Notify(shared_from_this(), "KILL_COILY");
		}
	}
}
