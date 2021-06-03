#include "pch.h"
#include "MainScene.h"
#include <SDL.h>
#include <imgui.h>
#include <backends\imgui_impl_sdl.h>
#include <backends\imgui_impl_opengl2.h>
#include <ResourceManager.h>
#include <GameObject.h>
#include <UIComponent.h>
#include <ServiceLocator.h>
#include "LevelScene.h"
#include <FPS.h>

void MainScene::Initialize()
{
	auto go = std::make_shared<GameObject>();
	auto texture = go->AddComponent<TextureComponent>();
	texture->SetTexture("background.jpg");
	Add(go);

	go = std::make_shared<GameObject>();
	texture = go->AddComponent<TextureComponent>();
	texture->SetTexture("logo.png");
	go->SetPosition(216, 180);
	Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<GameObject>();
	auto textComp = go->AddComponent<TextComponent>();
	textComp->SetText("QBERT");
	textComp->SetFont(font);
	go->SetPosition(80, 20);
	Add(go);

	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	go = std::make_shared<GameObject>();
	const auto fpsComponent = go->AddComponent<FPS>();
	fpsComponent->SetTextFontColor("FPS", font, SDL_Color{ 255, 255, 0 });
	Add(go);

	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
}

void MainScene::Update()
{
}

void MainScene::Render(SDL_Window* window) const
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	/*if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);*/
	int x, y;
	SDL_GetWindowSize(window, &x, &y);

	ImGui::SetWindowSize({ 120, 100 });
	ImGui::SetWindowPos({ float(x / 2 - 60), float(y / 2 + 50) });
	if (ImGui::Button("Single Player"))
	{
		LevelScene* newScene = new LevelScene("LevelScene");
		dae::SceneManager::GetInstance().AddScene(newScene);
		dae::SceneManager::GetInstance().SetActive("LevelScene");
		
	}
	if (ImGui::Button("Co-Op"))
	{
		LevelScene* newScene = new LevelScene("LevelScene");
		newScene->SetCoop();
		dae::SceneManager::GetInstance().AddScene(newScene);
		dae::SceneManager::GetInstance().SetActive("LevelScene");
	}
	if (ImGui::Button("Versus"))
	{
		LevelScene* newScene = new LevelScene("LevelScene");
		newScene->SetMultiplayer();
		dae::SceneManager::GetInstance().AddScene(newScene);
		dae::SceneManager::GetInstance().SetActive("LevelScene");
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
