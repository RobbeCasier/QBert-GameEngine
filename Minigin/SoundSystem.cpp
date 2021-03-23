#include "MiniginPCH.h"
#include "SoundSystem.h"
#include <cassert>

unsigned short SoundSystem::m_NrPending = 0;

void SDL_Sound_System::Update()
{
	//if equal, there is nothing to play
	if (m_Head == m_Tail)
		return;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return;

	//get filepath
	std::string assetFile = m_AudioList.at(m_Pending[m_Head].id);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	//switch between the two types of autio
	if (m_Pending[m_Head].isEffect)
	{
		//load chunk
		Mix_Chunk* effect = Mix_LoadWAV(assetFile.c_str());

		//is it loop or not
		if (m_Pending[m_Head].isLoop)
			Mix_PlayChannel(-1, effect, -1);
		else
			Mix_PlayChannel(-1, effect, 0);
	}
	else
	{
		//load music
		Mix_Music* music = Mix_LoadMUS(assetFile.c_str());

		//is it loop or not
		if (m_Pending[m_Head].isLoop)
			Mix_PlayMusic(music, -1);
		else
			Mix_PlayMusic(music, 0);
	}

	//set volume
	Mix_Volume(-1, m_Pending[m_Head].volume);

	//go to next sound in list
	m_Head = (m_Head + 1) % m_MaxPendingRequests;
}

void SDL_Sound_System::PlayMusic(const unsigned int& id, const unsigned int& volume, bool isLoop)
{
	assert((m_Tail + 1) % m_MaxPendingRequests != m_Head);

	m_Pending[m_Tail].id = id;
	m_Pending[m_Tail].volume = volume;
	m_Pending[m_Tail].isEffect = false;
	m_Pending[m_Tail].isLoop = isLoop;

	m_Tail = (m_Tail + 1) % m_MaxPendingRequests;
}


void SDL_Sound_System::PlayEffect(const unsigned int& id, const unsigned int& volume, bool isLoop)
{
	assert((m_Tail + 1) % m_MaxPendingRequests != m_Head);

	m_Pending[m_Tail].id = id;
	m_Pending[m_Tail].volume = volume;
	m_Pending[m_Tail].isEffect = true;
	m_Pending[m_Tail].isLoop = isLoop;

	m_Tail = (m_Tail + 1) % m_MaxPendingRequests;
}