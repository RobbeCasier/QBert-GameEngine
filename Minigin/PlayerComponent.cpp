#include "MiniginPCH.h"
#include "PlayerComponent.h"

void PlayerComponent::Initialize()
{
	m_ActorChanged = std::make_unique<Subject>();
}

void PlayerComponent::AddObserver(std::shared_ptr<Observer> observer)
{
	m_ActorChanged->AddObserver(observer);
}

void PlayerComponent::DecreaseHealth()
{
	--m_Health; 
	if (m_Health < 0) { m_Health = 0; }
	else
		m_ActorChanged->Notify(shared_from_this(), "IS_DEAD");
}

void PlayerComponent::AddScoreChangeColor()
{
	m_Score += 25;
	m_ActorChanged->Notify(shared_from_this(), "CHANGE_COLOR");
}

void PlayerComponent::AddScoreBeatCoily()
{
	m_Score += 500;
	m_ActorChanged->Notify(shared_from_this(), "BEAT_COILY");
}

void PlayerComponent::AddScoreRemainingDisc()
{
	m_Score += 50;
	m_ActorChanged->Notify(shared_from_this(), "REMAINING_DISC");
}

void PlayerComponent::AddScoreCatch()
{
	m_Score += 300;
	m_ActorChanged->Notify(shared_from_this(), "CATCH");
}
