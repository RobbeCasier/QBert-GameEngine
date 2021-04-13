#pragma once
#include "GameObject.h"
#include "TextComponent.h"

class PlayerComponent;
class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(std::shared_ptr<BaseComponent> actor, const std::string& event) = 0;
};