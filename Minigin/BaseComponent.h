#pragma once
#include "MiniginPCH.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>

#pragma warning(pop)
namespace dae {
	class GameObject;
}

using namespace dae;
class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent()
	{
		m_GameObject = nullptr;
	};
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;

	void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; };
protected:
	GameObject* m_GameObject;
};
