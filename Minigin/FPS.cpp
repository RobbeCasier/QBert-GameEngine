#include "MiniginPCH.h"
#include "FPS.h"

#include "GameObject.h"
#include "GameTime.h"
#include "TextComponent.h"

void FPS::Initialize()
{
	if (!m_GameObject->GetComponent<TextComponent>())
		m_GameObject->AddComponent<TextComponent>();
}

void FPS::Update()
{
	float deltaTime = Time::GetInstance().GetElapsedTime();
	int currentFPS = (int)(1 / deltaTime);
	if (currentFPS != m_FPS)
	{
		const std::string textString = std::to_string(currentFPS) + " FPS";
		m_GameObject->GetComponent<TextComponent>()->SetText(textString);
		m_FPS = currentFPS;
	}
}

void FPS::SetTextFontColor(const std::string& text, std::shared_ptr<Font>& font, const SDL_Color& color)
{
	std::shared_ptr<TextComponent> txtComp = m_GameObject->GetComponent<TextComponent>();
	txtComp->SetFont(font);
	txtComp->SetText(text);
	txtComp->SetColor(color);
}
