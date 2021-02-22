#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>

void dae::InputManager::AddInput( ControllerButton inputKey, std::unique_ptr<Command> command)
{
	m_ConsoleCommands.insert(std::pair<unsigned int, std::unique_ptr<Command>>((unsigned int)inputKey, std::move(command)));
}

bool dae::InputManager::ProcessInput()
{
	m_PreviousState = m_CurrentState;
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

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
	if (inputKey == m_CurrentState.Gamepad.wButtons
		&& m_PreviousState.Gamepad.wButtons != m_CurrentState.Gamepad.wButtons)
		return true;
	return false;
}

void dae::InputManager::HandleInput()
{
	for (auto& element : m_ConsoleCommands)
	{
		if (IsPressed(element.first))
		{
			element.second->Execute();
			continue;
		}
	}
}

