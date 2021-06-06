#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Transform.h"

namespace dae
{
	class TextComponent :
		public BaseComponent
	{
	public:
		explicit TextComponent() = default;
		virtual ~TextComponent() = default;

		virtual void Initialize() override;
		virtual void Update() override;
		void Render(const glm::vec3& position) override;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);
		void SetColor(const SDL_Color& color);
		void SetPosition(float x, float y);
	private:
		bool m_NeedsUpdate = true;
		SDL_Color m_Color = { 255, 255, 255 };
		std::string m_Text = "";
		std::shared_ptr<Font> m_Font = nullptr;
		std::shared_ptr<Transform> m_pTransformComponent;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}

