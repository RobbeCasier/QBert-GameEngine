#include "pch.h"
#include "PlayerManager.h"

void PlayerManager::AddPlayer(std::shared_ptr<Player> pPlayer)
{
	m_Players.push_back(pPlayer);
}

void PlayerManager::RemovePlayer(const int& id)
{
	m_Players.erase(m_Players.begin() + id);
}
