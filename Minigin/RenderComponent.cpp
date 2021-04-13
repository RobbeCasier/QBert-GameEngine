#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"

void RenderComponent::Render(const Texture2D& texture, const glm::vec3& position) const
{
	Renderer::GetInstance().RenderTexture(texture, position.x, position.y);
}

void RenderComponent::Render(const Texture2D& texture, const glm::vec3& position, float dstWidth, float dstHeight, const glm::vec2& src, float width, float height) const
{
	Renderer::GetInstance().RenderTexture(texture, position.x, position.y, dstWidth, dstHeight, src.x, src.y, width, height);
}
