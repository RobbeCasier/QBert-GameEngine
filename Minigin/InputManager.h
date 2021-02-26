#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Commands.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonUP = 0x0001,
		ButtonDOWN = 0x0002,
		ButtonLEFT = 0x0004,
		ButtonRIGHT = 0x0008,
		ButtonSTART = 0x0010,
		ButtonBACK = 0x0020,
		ButtonLEFTTHUMB = 0x0040,
		ButtonRIGHTTHUMB = 0x0080,
		ButtonLEFTSHOULDER = 0x0100,
		ButtonRIGHTSHOULDER = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,

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
