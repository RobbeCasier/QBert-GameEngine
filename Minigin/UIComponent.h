#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

class UIComponent :
    public BaseComponent
{
public:
    explicit UIComponent() = default;
    virtual ~UIComponent() = default;

    virtual void Initialize() override {};
    virtual void Update() override {}

    std::shared_ptr<TextComponent> AddTextComponent(const std::string& uiName, const std::string& text, std::shared_ptr<Font> font, float x = 0, float y = 0);
    std::shared_ptr<TextComponent> GetTextComponent(const std::string& uiName);
private:
    std::map<std::string, std::shared_ptr<BaseComponent>> m_UIElements;
};

