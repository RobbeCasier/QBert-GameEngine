#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->RootUpdate();
	}
}

void dae::SceneManager::Render(SDL_Window* window)
{
	for (const auto& scene : m_Scenes)
	{
		scene->RootRender(window);
	}
}

void dae::SceneManager::AddScene(Scene* scene)
{
	m_Scenes.push_back(std::shared_ptr<Scene>(scene));
	scene->RootInitialize();
}
