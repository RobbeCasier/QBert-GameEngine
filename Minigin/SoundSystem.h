#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <SDL_mixer.h>
#include <thread>
#include <mutex>

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
	SoundSystem()
	{
	}
	virtual ~SoundSystem() 
	{ 
		Mix_CloseAudio(); 
		m_WorkAvailable.notify_one();
	}

	virtual void LoadAndPlay() = 0;
	virtual void RegisterAudio(const unsigned int& id, const std::string& assetFile) { m_AudioList.insert(std::pair<unsigned int, std::string>(id, assetFile)); };
	
	virtual void Play(const unsigned int& id, const unsigned int& volume = 1.0f, bool isLoop = false, bool isEffect = true) = 0;
protected:
	static std::map<unsigned int, std::string> m_AudioList;

	static const unsigned short m_MaxPendingRequests = 16;

	static SoundRequest m_Pending[m_MaxPendingRequests];
	static unsigned short m_Head;
	static unsigned short m_Tail;

	static std::mutex m_Mutex;
	static std::condition_variable m_WorkAvailable;

};

class SDL_Sound_System : public SoundSystem
{
public:
	void LoadAndPlay() override;
	void Play(const unsigned int& id, const unsigned int& volume, bool isLoop = false, bool isEffect = true) override;

};

