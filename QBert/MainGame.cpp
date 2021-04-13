#include "pch.h"
#include "MainGame.h"
#include <SceneManager.h>
#include <InputManager.h>
#include <ServiceLocator.h>
#include <ResourceManager.h>
#include <Scene.h>
#include <FPS.h>
#include <UIComponent.h>
#include "MainScene.h"

void MainGame::LoadGame() const
{
	dae::SceneManager::GetInstance().AddScene(new MainScene("MainScreen"));
	ServiceLocator::RegisterSoundSystem(std::make_shared<SDL_Sound_System>(SDL_Sound_System()));
	auto& audio = ServiceLocator::GetSoundSystem();
	audio.RegisterAudio(1, "../sound/message_dada dam da dam.wav");

	//--------//
	//week 1-2//
	//--------//
}
