#include "pch.h"
#include "MainGame.h"
#include <SceneManager.h>
#include <InputManager.h>
#include <ServiceLocator.h>
#include <ResourceManager.h>
#include <Scene.h>
#include <FPS.h>
#include <UIComponent.h>
#include "MainMenuSDLButtons.h"
#include "QbertGameController.h"

void MainGame::LoadGame() const
{
	ServiceLocator::RegisterGameController(std::make_shared<QbertGameController>());
	ServiceLocator::GetGameController().Initialize();

	//load in sounds
	ServiceLocator::RegisterSoundSystem(std::make_shared<SDL_Sound_System>(SDL_Sound_System()));
	auto& audio = ServiceLocator::GetSoundSystem();
	audio.RegisterAudio(1, "../sound/jump.wav");
	audio.RegisterAudio(2, "../sound/jump-2.wav");
	audio.RegisterAudio(3, "../sound/jump-3.wav");
	audio.RegisterAudio(4, "../sound/jump-4.wav");
	audio.RegisterAudio(5, "../sound/tune-2.wav");
	audio.RegisterAudio(6, "../sound/lift.wav");
	audio.RegisterAudio(7, "../sound/fall.wav");
	audio.RegisterAudio(8, "../sound/snake-fall.wav");
	audio.RegisterAudio(9, "../sound/speech.wav");
	audio.RegisterAudio(10, "../sound/swear.wav");
}
