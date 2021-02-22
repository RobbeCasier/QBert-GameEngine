#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"


namespace dae
{
	class TextComponent :
		public BaseComponent
	{
	public:
		explicit TextComponent() = default;
		virtual ~TextComponent() = default;

		virtual void Initialize() override {};
		virtual void Update() override;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);
		void SetColor(const SDL_Color& color);
	private:
		bool m_NeedsUpdate = true;
		SDL_Color m_Color = { 255, 255, 255 };
		std::string m_Text = "";
		std::shared_ptr<Font> m_Font = nullptr;
	};
}

