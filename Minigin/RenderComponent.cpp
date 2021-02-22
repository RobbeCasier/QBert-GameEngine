#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"

void RenderComponent::Render(const glm::vec3& position) const
{
	Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
}
