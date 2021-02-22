#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Commands.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	using ControllerCommandsMap = std::map<unsigned int, std::unique_ptr<Command>>;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void AddInput(ControllerButton button, std::unique_ptr<Command> command);
		bool ProcessInput();
		bool IsPressed(unsigned int inputKey) const;
		void HandleInput();
	private:
		ControllerCommandsMap m_ConsoleCommands{};
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
	};

}
