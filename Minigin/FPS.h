#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <SDL.h>

class FPS : public BaseComponent
{
public:
	FPS() = default;
	virtual ~FPS() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	void SetTextFontColor(const std::string& text, std::shared_ptr<Font>& font, const SDL_Color& color);
private:
	int m_FPS = 0;
};

