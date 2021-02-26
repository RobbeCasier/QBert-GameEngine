#include "MiniginPCH.h"
#include "UIComponent.h"
#include "GameObject.h"

std::shared_ptr<TextComponent> UIComponent::AddTextComponent(const std::string& uiName, const std::string& text, std::shared_ptr<Font> font, float x, float y)
{
    std::shared_ptr<TextComponent> component = m_GameObject->AddComponent<TextComponent>();
    component->SetFont(font);
    component->SetText(text);
    component->SetPosition(x, y);
    m_UIElements.insert(std::pair<std::string, std::shared_ptr<BaseComponent>>(uiName, component));
    return component;
}

std::shared_ptr<TextComponent> UIComponent::GetTextComponent(const std::string& uiName)
{
    auto component = std::find_if(m_UIElements.begin(), m_UIElements.end(),
        [uiName](std::pair<std::string, std::shared_ptr<BaseComponent>> component)
        {
            return component.first == uiName;
        });
    if (component != m_UIElements.end())
        return std::dynamic_pointer_cast<TextComponent>(component->second);
    return nullptr;
}
