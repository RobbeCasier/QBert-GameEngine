#pragma once
#include "SoundSystem.h"

class ServiceLocator final
{
public:
	static SoundSystem& GetSoundSystem() { return *m_SoundSystem; };
	static void RegisterSoundSystem(std::shared_ptr<SoundSystem> ss) { m_SoundSystem = ss; };
private:
	inline static std::shared_ptr<SoundSystem> m_SoundSystem = nullptr;
};