#pragma once
#include "Singleton.h"

struct SDL_Window;
namespace dae
{
	class GameContext final : public Singleton<GameContext>
	{
	public:
		void SetWindow(SDL_Window* window)
		{
			m_pWindow = window;
		}
		SDL_Window* GetWindow() const
		{
			return m_pWindow;
		}
	private:
		friend class Singleton<GameContext>;
		GameContext() = default;
		SDL_Window* m_pWindow = nullptr;
	};
}