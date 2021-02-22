#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::GameObject::Initialize()
{
	m_pTransformComponent = AddComponent<Transform>();
	m_pRenderComponent = AddComponent<RenderComponent>();
	m_pTextureComponent = AddComponent<TextureComponent>();
}

void dae::GameObject::Update()
{
	if (m_Components.size() != 0)
	{
		std::for_each(m_Components.begin(), m_Components.end(),
			[](BaseComponent* component)
			{
				component->Update();
			});
	}
}

void dae::GameObject::Render() const
{
	m_pRenderComponent->Render(m_pTransformComponent->GetPosition());
}

void dae::GameObject::SetTexture(const std::string& fileName)
{
	m_pTextureComponent->SetTexture(fileName);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_pTransformComponent->SetPosition(x, y, 0.f);
}

