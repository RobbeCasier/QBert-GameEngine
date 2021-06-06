#include "MiniginPCH.h"
#include "InputManager.h"
#include "SDL.h"

dae::InputManager::InputManager()
{
	//initialize
	//assign pointer
	m_CurrentKeyboardState = SDL_GetKeyboardState(NULL);
}

void dae::InputManager::AddController()
{
	m_Controllers.push_back(new Controller((unsigned int)m_Controllers.size()));
}

void dae::InputManager::AddInput( ControllerButton inputKey, std::unique_ptr<Command> command, const unsigned int& player)
{
	UNREFERENCED_PARAMETER(inputKey);
	UNREFERENCED_PARAMETER(player);
	if (m_Controllers.size() > player)
	{
		auto itr = m_Controllers[player]->controllerCommandMap.find((int)inputKey);
		if (itr == m_Controllers[player]->controllerCommandMap.end())
			m_Controllers[player]->controllerCommandMap.insert(std::pair<unsigned int, std::unique_ptr<Command>>((unsigned int)inputKey, std::move(command)));
		else
			m_Controllers[player]->controllerCommandMap.at((int)inputKey) = std::move(command);
	}
}

void dae::InputManager::AddInput(KeyboardKeys key, std::unique_ptr<Command> command)
{
	auto itr = m_KeyboardCommands.find((int)key);
	if (itr == m_KeyboardCommands.end())
		m_KeyboardCommands.insert(std::pair<unsigned int, std::unique_ptr<Command>>((unsigned int)key, std::move(command)));
	else
		m_KeyboardCommands.at((int)key) = std::move(command);
}

bool dae::InputManager::ProcessInput()
{
	//update states
	//controller
	for (auto& controller : m_Controllers)
	{
		if (controller->IsConnected())
		{
			controller->previousState = controller->currentState;
			controller->UpdateState();
			continue;
		}
		std::cout << "No Controller detected" << std::endl;
	}

	//keyboard
	memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, SDL_NUM_SCANCODES);
	m_CurrentKeyboardState = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

bool dae::InputManager::IsPressed(unsigned int inputKey) const
{
	if (m_CurrentKeyboardState[inputKey]
		&& m_PreviousKeyboardState[inputKey] != m_CurrentKeyboardState[inputKey])
		return true;
	return false;
}

void dae::InputManager::HandleInput()
{
	for (auto& controller : m_Controllers)
	{
		for (auto& element : controller->controllerCommandMap)
		{
			if (controller->IsPressed(element.first))
			{
				element.second->Execute();
				continue;
			}
		}
	}

	for (auto& element : m_KeyboardCommands)
	{
		if (IsPressed(element.first))
		{
			element.second->Execute();
			continue;
		}
	}
}

