#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

void TextureComponent::Initialize()
{
	m_TransformComponent = std::make_shared<Transform>();
}

void TextureComponent::Render(std::shared_ptr<RenderComponent> renderComponent, const glm::vec3& position)
{
	glm::vec3 worldPos = m_TransformComponent->GetPosition() + position;
	renderComponent->Render(*m_Texture, worldPos);
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureComponent::SetPosition(float x, float y)
{
	m_TransformComponent->SetPosition(x, y, 0.f);
}
