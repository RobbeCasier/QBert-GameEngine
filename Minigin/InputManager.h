#pragma once
#include <XInput.h>
#include <WinUser.h>
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

	enum class KeyboardKeys
	{
		W = SDL_SCANCODE_W,
		S = SDL_SCANCODE_S,
		A = SDL_SCANCODE_A,
		D = SDL_SCANCODE_D,
		Up = SDL_SCANCODE_UP,
		Down = SDL_SCANCODE_DOWN,
		Left = SDL_SCANCODE_LEFT,
		Right = SDL_SCANCODE_RIGHT,
		Esc = SDL_SCANCODE_ESCAPE
	};


	using ControllerCommandsMap = std::map<unsigned int, std::unique_ptr<Command>>;

	//controller
	struct Controller
	{
		Controller(const unsigned int& playerNumber): number{playerNumber}{}

		void UpdateState()
		{
			ZeroMemory(&currentState, sizeof(XINPUT_STATE));
			XInputGetState(number, &currentState);
		}
		bool IsConnected()
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			DWORD result = XInputGetState(number, &state);

			if (result == ERROR_SUCCESS)
				return true;
			return false;
		}
		bool IsPressed(unsigned int inputKey)
		{
			if (inputKey == currentState.Gamepad.wButtons
				&& previousState.Gamepad.wButtons != currentState.Gamepad.wButtons)
				return true;
			return false;
		}

		ControllerCommandsMap controllerCommandMap;
		XINPUT_STATE previousState;
		XINPUT_STATE currentState;
		unsigned int number;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		void AddController();
		void AddInput(ControllerButton button, std::unique_ptr<Command> command, const unsigned int& player);
		void AddInput(KeyboardKeys key, std::unique_ptr<Command> command);
		bool ProcessInput();
		void HandleInput();
	private:
		bool IsPressed(unsigned int inputKey) const;

		std::vector<Controller*> m_Controllers;
		ControllerCommandsMap m_KeyboardCommands{};

		const Uint8* m_CurrentKeyboardState = nullptr;
		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ 0 };
	};

}
