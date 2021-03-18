#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <SDL_mixer.h>

class SoundSystem
{
public:
	SoundSystem() {}
	virtual ~SoundSystem() { Mix_CloseAudio(); };
	virtual void PlayEffect(const unsigned int& id, const int& volume, bool isLoop = false) = 0;
	virtual void PlayMusic(const unsigned int& id, const int& volume, bool isLooop = false) = 0;
	virtual void RegisterAudio(const unsigned int& id, const std::string& assetFile) { m_AudioList.insert(std::pair<unsigned int, std::string>(id, assetFile)); };
protected:
	std::map<unsigned int, std::string> m_AudioList;
};

class SDL_Sound_System : public SoundSystem
{
public:
	void PlayEffect(const unsigned int& id, const int& volume, bool isLoop = false) override;
	void PlayMusic(const unsigned int& id, const int& volume, bool isLoop = false) override;
};

