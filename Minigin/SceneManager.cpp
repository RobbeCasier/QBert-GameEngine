#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for (auto& scene : m_Scenes)
	{
		if (scene->IsActive())
			scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (auto& scene : m_ActiveScenes)
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

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(const std::string& name)
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetSceneName()._Equal(name))
			return scene;
	}
	return nullptr;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	for (int indx = 0; indx < (int)m_Scenes.size(); ++indx)
	{
		if (m_Scenes[indx]->GetSceneName()._Equal(name))
		{
			m_Scenes.erase(m_Scenes.begin() + indx);
			break;
		}
	}
	for (int indx = 0; indx < (int)m_ActiveScenes.size(); ++indx)
	{
		if (m_ActiveScenes[indx]->GetSceneName()._Equal(name))
		{
			m_ActiveScenes.erase(m_ActiveScenes.begin() + indx);
			return;
		}
	}
}

void dae::SceneManager::LoadScene(const std::string& name)
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetSceneName()._Equal(name))
		{
			scene->UnPause();
			m_ActiveScenes.push_back(scene);
			return;
		}
	}
}

void dae::SceneManager::UnloadScene(const std::string& name)
{
	for (int indx = 0; indx < (int)m_ActiveScenes.size(); ++indx)
	{
		if (m_ActiveScenes[indx]->GetSceneName()._Equal(name))
		{
			m_ActiveScenes.erase(m_ActiveScenes.begin() + indx);
			return;
		}
	}
}
