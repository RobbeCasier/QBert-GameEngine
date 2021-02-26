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
#include "TextureComponent.h"
#include "FPS.h"
#include "Time.h"
#include "Commands.h"
#include "PlayerComponent.h"
#include "Observer.h"
#include "UIComponent.h"

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
	auto& input = InputManager::GetInstance();

	//--------//
	//week 1-2//
	//--------//
	auto go = std::make_shared<GameObject>();
	auto texture = go->AddComponent<TextureComponent>();
	texture->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	texture = go->AddComponent<TextureComponent>();
	texture->SetTexture("logo.png");
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

	//------//
	//week 3//
	//------//
	//ui object
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	////ui 1
	go = std::make_shared<GameObject>();
	go->SetPosition(0, 50);
	auto ui = go->AddComponent<UIComponent>();
	//lives 1
	auto text = ui->AddTextComponent("Lives", "Lives: 3", font);
	std::shared_ptr<LivesDisplay> livesDisplayP1 = std::make_shared<LivesDisplay>(text);
	//score
	text = ui->AddTextComponent("Score", "0", font, 0, 10);
	std::shared_ptr<ScoreDisplay> scoreDisplayP1 = std::make_shared<ScoreDisplay>(text);
	scene.Add(go);

	////ui 2
	go = std::make_shared<GameObject>();
	go->SetPosition(540, 50);
	ui = go->AddComponent<UIComponent>();
	//live 2
	text = ui->AddTextComponent("Lives", "Lives: 3", font);
	std::shared_ptr<LivesDisplay> livesDisplayP2 = std::make_shared<LivesDisplay>(text);
	//score 2
	text = ui->AddTextComponent("Score", "0", font, 0, 10);
	std::shared_ptr<ScoreDisplay> scoreDisplayP2 = std::make_shared<ScoreDisplay>(text);
	scene.Add(go);

	//player 1
	go = std::make_shared<GameObject>();
	auto player = go->AddComponent<PlayerComponent>();
	player->AddObserver(livesDisplayP1);
	player->AddObserver(scoreDisplayP1);
	input.AddInput(ControllerButton::ButtonLEFTTHUMB, std::make_unique<InstantDeath>(player));
	input.AddInput(ControllerButton::ButtonUP, std::make_unique<ChangeColor>(player));
	input.AddInput(ControllerButton::ButtonRIGHT, std::make_unique<BeatCoily>(player));
	input.AddInput(ControllerButton::ButtonDOWN, std::make_unique<RemainingDisc>(player));
	input.AddInput(ControllerButton::ButtonLEFT, std::make_unique<Catch>(player));
	scene.Add(go);

	//player 2
	go = std::make_shared<GameObject>();
	player = go->AddComponent<PlayerComponent>();
	player->AddObserver(livesDisplayP2);
	player->AddObserver(scoreDisplayP2);
	input.AddInput(ControllerButton::ButtonRIGHTTHUMB, std::make_unique<InstantDeath>(player));
	input.AddInput(ControllerButton::ButtonY, std::make_unique<ChangeColor>(player));
	input.AddInput(ControllerButton::ButtonB, std::make_unique<BeatCoily>(player));
	input.AddInput(ControllerButton::ButtonA, std::make_unique<RemainingDisc>(player));
	input.AddInput(ControllerButton::ButtonX, std::make_unique<Catch>(player));
	scene.Add(go);

	//control UI
	go = std::make_shared<GameObject>();
	ui = go->AddComponent<UIComponent>();
	go->SetPosition(200, 60);
	ui->AddTextComponent("Death", "Death: Left Thumb/Right Thumb", font);
	ui->AddTextComponent("Color Change", "Color Change : Up / Y", font, 0, 12);
	ui->AddTextComponent("Beat Coily", "Beat Coily : Right / B", font, 0, 24);
	ui->AddTextComponent("Remaining Disc", "Remaining Disc : Down / A", font, 0, 36); 
	ui->AddTextComponent("Catch", "Catch: Left / X", font, 0, 48);
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
			input.HandleInput();

			sceneManager.Update();
			renderer.Render();

			const auto sleepTime = currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now();
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
