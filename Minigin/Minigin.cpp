#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FPS.h"
#include "Time.h"

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

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	go->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<GameObject>();
	auto textComp = go->AddComponent<TextComponent>();
	textComp->SetText("Programming 4 Assignment");
	textComp->SetFont(font);
	go->SetPosition(80, 20);
	scene.Add(go);

	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	go = std::make_shared<GameObject>();
	const auto fpsComponent = go->AddComponent<FPS>();
	fpsComponent->SetTextFontColor("FPS", font, SDL_Color{255, 255, 0});
	scene.Add(go);
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
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

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

			sceneManager.Update();
			renderer.Render();

			const auto sleepTime = currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now();
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
