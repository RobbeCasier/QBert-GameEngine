#include "pch.h"
#include "MainMenuLevel.h"
#include <SceneManager.h>
#include <ResourceManager.h>
#include <Scene.h>
#include <TextureComponent.h>
#include <TextComponent.h>
#include <FPS.h>
#include "MainMenuSDLButtons.h"
#include <GameObject.h>

void MainMenuLevel::Initialize()
{
	//MainMenu
	m_pMainMenuScene = dae::SceneManager::GetInstance().CreateScene("MainMenu");

	//background
	auto go = std::make_shared<GameObject>();
	//add texturecomponent
	auto texture = go->AddComponent<TextureComponent>();
	//set texture
	texture->SetTexture("background.jpg");
	//add to scene
	m_pMainMenuScene->AddObject(go);

	//logo
	go = std::make_shared<GameObject>();
	texture = go->AddComponent<TextureComponent>();
	texture->SetTexture("logo.png");
	go->SetPosition(216, 180);
	m_pMainMenuScene->AddObject(go);

	//title
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<GameObject>();
	auto textComp = go->AddComponent<TextComponent>();
	textComp->SetText("QBERT");
	textComp->SetFont(font);
	go->SetPosition(80, 20);
	m_pMainMenuScene->AddObject(go);

	//fps
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	go = std::make_shared<GameObject>();
	const auto fpsComponent = go->AddComponent<FPS>();
	fpsComponent->SetTextFontColor("FPS", font, SDL_Color{ 255, 255, 0 });
	m_pMainMenuScene->AddObject(go);

	//SDL
	go = std::make_shared<GameObject>();
	auto sdlMenu = go->AddComponent<MainMenuSDLButtons>();
	m_pMainMenuScene->AddObject(go);
}

void MainMenuLevel::Update()
{
}

void MainMenuLevel::Deactivate()
{
	dae::SceneManager::GetInstance().RemoveScene("MainMenu");
}
