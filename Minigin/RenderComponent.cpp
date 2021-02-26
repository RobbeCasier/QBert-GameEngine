#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"

void RenderComponent::Render(const Texture2D& texture, const glm::vec3& position) const
{
	Renderer::GetInstance().RenderTexture(texture, position.x, position.y);
}
