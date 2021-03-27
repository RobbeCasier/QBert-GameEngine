#include "MiniginPCH.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"

void LivesDisplay::OnNotify(std::shared_ptr<PlayerComponent> actor, const std::string& event)
{
	if (event == "IS_DEAD")
	{
		std::cout << "PlayerDied" << std::endl;
		m_UI->SetText("Lives: "+std::to_string(actor->GetHealth()));
	}
}

void ScoreDisplay::OnNotify(std::shared_ptr<PlayerComponent> actor, const std::string& event)
{
	if (event == "CHANGE_COLOR")
	{
		std::cout << "Changed Color" << std::endl;
		m_UI->SetText(std::to_string(actor->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
	else if (event == "BEAT_COILY")
	{
		std::cout << "Beat Coily" << std::endl;
		m_UI->SetText(std::to_string(actor->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
	else if (event == "REMAINING_DISC")
	{
		std::cout << "Remaining disc" << std::endl;
		m_UI->SetText(std::to_string(actor->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
	else if (event == "CATCH")
	{
		std::cout << "Catched Slick/Sam" << std::endl;
		m_UI->SetText(std::to_string(actor->GetScore()));
		ServiceLocator::GetSoundSystem().Play(1, 100);
	}
}
