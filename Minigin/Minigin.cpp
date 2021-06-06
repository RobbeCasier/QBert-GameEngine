#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "LevelManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FPS.h"
#include "GameTime.h"
#include "Commands.h"
#include "PlayerComponent.h"
#include "Observer.h"
#include "UIComponent.h"
#include "ServiceLocator.h"
#include "GameContext.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	GameContext::GetInstance().SetWindow(m_Window);
	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	

}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& gameController = ServiceLocator::GetGameController();
		auto& sceneManager = SceneManager::GetInstance();
		auto& levelManager = LevelManager::GetInstance();
		auto& input = InputManager::GetInstance();

		auto& audio = ServiceLocator::GetSoundSystem();
		std::thread audioThread(&SoundSystem::LoadAndPlay, &audio);
		audioThread.detach();

		auto& time = Time::GetInstance();
		bool doContinue = true;
		auto lastTime = high_resolution_clock::now();
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			const float deltaTime = duration<float>(currentTime - lastTime).count();
			time.SetElapsedTime(deltaTime);
			lastTime = currentTime;
			doContinue = input.ProcessInput();
			input.HandleInput();

			gameController.Update();
			levelManager.Update();
			sceneManager.Update();
			renderer.Render();

			const auto sleepTime = currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now();
			this_thread::sleep_for(sleepTime);
		}
	}
	Cleanup();
}
