#pragma once
#include <Singleton.h>
#include "Player.h"

class PlayerManager : public dae::Singleton<PlayerManager>
{
public:
	void AddPlayer(std::shared_ptr<Player> pPlayer);
	void RemovePlayer(const int& id);
	std::vector<std::shared_ptr<Player>> GetPlayers() { return m_Players; }
private:
	friend class Singleton<PlayerManager>;
	PlayerManager() = default;
	std::vector<std::shared_ptr<Player>> m_Players;
};