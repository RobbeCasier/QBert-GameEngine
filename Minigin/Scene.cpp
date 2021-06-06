#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {  }

Scene::~Scene() = default;

void Scene::AddObject(std::shared_ptr<GameObject> object)
{
	object->SetScene(shared_from_this());
	m_ObjectsToAdd.push_back(object);
}

void dae::Scene::RemoveObject(std::shared_ptr<GameObject> object)
{
	m_ObjectsToDelete.push_back(object);
}

void dae::Scene::Add()
{
	for (auto object : m_ObjectsToAdd)
	{
		m_Objects.push_back(object);
	}
	m_ObjectsToAdd.clear();
}

void dae::Scene::Remove()
{
	for (auto object : m_ObjectsToDelete)
	{
		auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (it != m_Objects.end())
		{
			m_Objects.erase(it);
			object.reset();
		}
	}
	m_ObjectsToDelete.clear();
}

void dae::Scene::Update()
{
	Add();
	for (auto& object : m_Objects)
	{
		object->Update();
	}
	Remove();
}

void dae::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

