#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for (auto scene : m_Scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (auto scene : m_Scenes)
	{
		scene->Render();
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::CreateScene(const std::string& sceneName)
{
	const auto scene = std::shared_ptr<Scene>(new dae::Scene(sceneName));
	m_Scenes.push_back(scene);
	return scene;
}

void dae::SceneManager::LoadScene(const std::string& name)
{
	for (auto scene : m_Scenes)
	{
		if (scene->GetSceneName()._Equal(name))
		{
			m_ActiveScenes.push_back(scene);
			return;
		}
	}
}

void dae::SceneManager::UnloadScene(const std::string& name)
{
	for (int indx = 0; indx < m_ActiveScenes.size(); ++indx)
	{
		if (m_ActiveScenes[indx]->GetSceneName()._Equal(name))
		{
			m_ActiveScenes.erase(m_ActiveScenes.begin() + indx);
			return;
		}
	}
	for (int indx = 0; indx < m_Scenes.size(); ++indx)
	{
		if (m_Scenes[indx]->GetSceneName()._Equal(name))
		{
			m_Scenes.erase(m_Scenes.begin() + indx);
			return;
		}
	}
}
