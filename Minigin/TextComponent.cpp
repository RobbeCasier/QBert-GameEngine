#include "MiniginPCH.h"
#include "TextComponent.h"
#include "GameObject.h"

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		RenderComponent* renderComp = m_GameObject->GetComponent<RenderComponent>();
		renderComp->SetTexture(std::make_shared<Texture2D>(texture));
		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_Font = font;
	m_NeedsUpdate = true;
}

void TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}

