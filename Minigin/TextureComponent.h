#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "BaseComponent.h"
#include "Texture2D.h"
#include "RenderComponent.h"

using namespace dae;
	class TextureComponent final :
	    public BaseComponent
	{
	public:
		explicit TextureComponent() = default;
		virtual ~TextureComponent() = default;

		virtual void Initialize() override {};
		virtual void Update() override {};

		void SetTexture(const std::string& filename);
	};

