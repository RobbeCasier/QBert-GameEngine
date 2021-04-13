#pragma once
#include "Observer.h"
#include "GameObject.h"
#include <vector>

class Subject
{
public:
	void AddObserver(std::shared_ptr<Observer> observer)
	{
		m_Observers.push_back(observer);
	}

	void RemoveObserver(std::shared_ptr<Observer> observer)
	{
		auto observerLocation = std::find(m_Observers.begin(), m_Observers.end(), observer);
		m_Observers.erase(observerLocation);
	}

	void Notify(std::shared_ptr<BaseComponent> actor, const std::string& event)
	{
		for (auto &element: m_Observers)
		{
			element->OnNotify(actor, event);
		}
	}
private:
	std::vector<std::shared_ptr<Observer>> m_Observers;
};