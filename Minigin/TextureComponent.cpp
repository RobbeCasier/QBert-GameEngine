#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <SDL_opengl.h>
#include <SDL.h>

void TextureComponent::Initialize()
{
	m_TransformComponent = std::make_shared<Transform>();
}

void TextureComponent::Render(std::shared_ptr<RenderComponent> renderComponent, const glm::vec3& position)
{
	glm::vec3 worldPos = m_TransformComponent->GetPosition() + position;
	if (m_HasSourceRect)
		renderComponent->Render(*m_Texture, worldPos, m_Width, m_Height, glm::vec2{ static_cast<float>(m_SrcRect.x), static_cast<float>(m_SrcRect.y) }, static_cast<float>(m_SrcRect.w), static_cast<float>(m_SrcRect.h));
	else
		renderComponent->Render(*m_Texture, worldPos);
}

void TextureComponent::SetTexture(const std::string& filename, int width, int height)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);

	if (width == 0)
		return;
	if (height == 0)
		m_Height = (float)width;
	else
		m_Height = (float)height;
	m_Width = (float)width;
}

void TextureComponent::SetPosition(float x, float y)
{
	m_TransformComponent->SetPosition(x, y, 0.f);
}

void TextureComponent::SetSource(const int& col, const int& row, const int& nrCol, const int& nrRow)
{
	int w, h;
	SDL_QueryTexture(m_Texture->GetSDLTexture(), nullptr, nullptr, &w, &h);
	//bottomLeft
	m_SrcRect.x = w / nrCol * col;
	m_SrcRect.y = (h / nrRow * (row-1)) + h / nrRow;
	m_SrcRect.w = w / nrCol;
	m_SrcRect.h = h / nrRow;
	m_HasSourceRect = true;
}
