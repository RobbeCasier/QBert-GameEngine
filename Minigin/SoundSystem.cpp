#include "MiniginPCH.h"
#include "SoundSystem.h"

void SDL_Sound_System::PlayMusic(const unsigned int& id, const int& volume, bool isLoop)
{
	if (SDL_Init(SDL_INIT_AUDIO)<0)
		return;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	//get file from map and create chunk
	std::string assetFile = m_AudioList.at(id);
	Mix_Music* sound = Mix_LoadMUS(assetFile.c_str());

	//play on channel
	if (isLoop)
		Mix_PlayMusic(sound, -1);
	else
		Mix_PlayMusic(sound, 0);

	//set volume
	Mix_Volume(-1, volume);
}

void SDL_Sound_System::PlayEffect(const unsigned int& id, const int& volume, bool isLoop)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	//get file from map and create chunk
	std::string assetFile = m_AudioList.at(id);
	Mix_Chunk* sound = Mix_LoadWAV(assetFile.c_str());

	//play on channel
	if (isLoop)
		Mix_PlayChannel(-1,sound, -1);
	else
		Mix_PlayChannel(-1, sound, 0);

	//set volume
	Mix_Volume(-1, volume);
}
