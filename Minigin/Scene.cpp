#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {  }

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void dae::Scene::Remove(std::shared_ptr<GameObject> object)
{
	auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
	if (it != m_Objects.end())
	{
		m_Objects.erase(it);
		object.reset();
	}
}

void dae::Scene::RootInitialize()
{
	Initialize();
}

void dae::Scene::RootUpdate()
{
	for (auto& object : m_Objects)
	{
		object->Update();
	}
	Update();
}

void dae::Scene::RootRender(SDL_Window* window) const
{
	UNREFERENCED_PARAMETER(window);
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
	Render(window);
}

