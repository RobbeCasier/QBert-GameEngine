#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <SDL_opengl.h>
#include <SDL.h>

void TextureComponent::Initialize()
{
	m_pTransformComponent = std::make_shared<Transform>();
}

void TextureComponent::Render(const glm::vec3& position)
{
	auto transform = GetGameObject()->GetComponent<Transform>();
	auto render = GetGameObject()->GetComponent<RenderComponent>();
	glm::vec3 worldPos = m_pTransformComponent->GetPosition() + position;
	if (m_HasSourceRect)
		render->Render(*m_pTexture, worldPos, m_Width, m_Height, glm::vec2{ static_cast<float>(m_SrcRect.x), static_cast<float>(m_SrcRect.y) }, static_cast<float>(m_SrcRect.w), static_cast<float>(m_SrcRect.h));
	else
		render->Render(*m_pTexture, worldPos);
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureComponent::SetPosition(float x, float y)
{
	m_pTransformComponent->SetPosition(x, y, 0.f);
}

void TextureComponent::SetSource(const int& col, const int& row, const int& nrCol, const int& nrRow)
{
	int w, h;
	SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &w, &h);
	//bottomLeft
	m_SrcRect.x = w / nrCol * col;
	m_SrcRect.y = (h / nrRow * (row-1)) + h / nrRow;
	m_SrcRect.w = w / nrCol;
	m_SrcRect.h = h / nrRow;
	m_HasSourceRect = true;
}

void TextureComponent::SetSize(const int& width, const int& height)
{
	m_Height = (float)height;
	m_Width = (float)width;
}

int TextureComponent::GetWidth()
{
	int x, y;
	SDL_QueryTexture(m_pTexture->GetSDLTexture(), 0, 0, &x, &y);
	return x;
}

int TextureComponent::GetHeight()
{
	int x, y;
	SDL_QueryTexture(m_pTexture->GetSDLTexture(), 0, 0, &x, &y);
	return y;
}
