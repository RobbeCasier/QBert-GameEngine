#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "BaseComponent.h"
#include "Texture2D.h"
#include "RenderComponent.h"
#include "Transform.h"

using namespace dae;
	class TextureComponent final :
	    public BaseComponent
	{
	public:
		explicit TextureComponent() = default;
		virtual ~TextureComponent() = default;

		virtual void Initialize() override;
		virtual void Update() override {};
		void Render(std::shared_ptr<RenderComponent> renderComponent, const glm::vec3& position);

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<Transform> m_TransformComponent;
	};

