#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <SDL_mixer.h>

struct SoundRequest
{
	unsigned int id;
	unsigned int volume;
	bool isLoop;
	bool isEffect; //to see if a chuck or music has to be loaded
};

class SoundSystem
{
public:
	SoundSystem() {}
	virtual ~SoundSystem() { Mix_CloseAudio(); };
	virtual void Update() = 0;
	virtual void PlayEffect(const unsigned int& id, const unsigned int& volume, bool isLoop = false) = 0;
	virtual void PlayMusic(const unsigned int& id, const unsigned int& volume, bool isLoop = false) = 0;
	virtual void RegisterAudio(const unsigned int& id, const std::string& assetFile) { m_AudioList.insert(std::pair<unsigned int, std::string>(id, assetFile)); };
protected:
	static std::map<unsigned int, std::string> m_AudioList;

	static const unsigned short m_MaxPendingRequests = 32;

	static SoundRequest m_Pending[m_MaxPendingRequests];
	static unsigned short m_Head;
	static unsigned short m_Tail;
};

class SDL_Sound_System : public SoundSystem
{
public:
	void Update() override;
	void PlayEffect(const unsigned int& id, const unsigned int& volume, bool isLoop = false) override;
	void PlayMusic(const unsigned int& id, const unsigned int& volume, bool isLoop = false) override;

};

