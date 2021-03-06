#pragma once
#include "MiniginPCH.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#pragma warning(pop)

namespace dae {
	class GameObject;
}

class RenderComponent;
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
	virtual void Render(const glm::vec3& position) {UNREFERENCED_PARAMETER(position);}
	void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; };
	GameObject* GetGameObject() { return m_GameObject; }
protected:
	GameObject* m_GameObject = nullptr;
};

