#pragma once
#include <Commands.h>
#include "Grid.h"

class ChangeColorGrid final :public Command
{
public:
	explicit ChangeColorGrid(std::shared_ptr<Grid> actor) : Command(actor) {}
	void Execute() override
	{
		std::dynamic_pointer_cast<Grid>(m_Actor)->CheckCompletion();
	}
};