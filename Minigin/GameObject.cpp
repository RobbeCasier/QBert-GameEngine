#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::GameObject::Initialize()
{
	m_pTransformComponent = AddComponent<Transform>();
	m_pRenderComponent = AddComponent<RenderComponent>();
}

void dae::GameObject::Update()
{
	if (m_Components.size() != 0)
	{
		std::for_each(m_Components.begin(), m_Components.end(),
			[](std::shared_ptr<BaseComponent> component)
			{
				component->Update();
			});
	}
}

void dae::GameObject::Render() const
{
	for (auto element: m_Components)
	{
		element->Render(m_pRenderComponent, m_pTransformComponent->GetPosition());
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_pTransformComponent->SetPosition(x, y, 0.f);
}

