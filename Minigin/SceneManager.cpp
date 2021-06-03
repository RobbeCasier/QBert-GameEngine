#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_ActiveScene->RootUpdate();
}

void dae::SceneManager::Render(SDL_Window* window)
{
	m_ActiveScene->RootRender(window);
}

void dae::SceneManager::AddScene(Scene* scene)
{
	m_Scenes.push_back(std::shared_ptr<Scene>(scene));
	scene->RootInitialize();
}

void dae::SceneManager::SetActive(const std::string& sceneName)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
		[sceneName](std::shared_ptr<Scene> scene)
		{
			return scene->GetSceneName() == sceneName;
		}
	);
	if (it != m_Scenes.end())
	{
		m_ActiveScene = *it;
	}
}
