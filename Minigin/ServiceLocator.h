#pragma once
#include "SoundSystem.h"
#include "GameController.h"

class ServiceLocator final
{
public:
	static SoundSystem& GetSoundSystem() { return *m_SoundSystem; };
	static void RegisterSoundSystem(std::shared_ptr<SoundSystem> ss) { m_SoundSystem = ss; };

	static GameController& GetGameController() { return *m_GameController; }
	static void RegisterGameController(std::shared_ptr<GameController> gc) { m_GameController = gc; }
private:
	inline static std::shared_ptr<SoundSystem> m_SoundSystem = nullptr;
	inline static std::shared_ptr<GameController> m_GameController = nullptr;
};