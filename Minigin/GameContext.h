#pragma once
#include "Singleton.h"

enum class GameState
{
	PLAY,
	WIN,
	GREENBALL,
	COLLISION
};
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
		GameState GetGameState() { return m_GameState; }
		void Play() { m_GameState = GameState::PLAY; }
		void Win() { m_GameState = GameState::WIN; }
		void Collide() { m_GameState = GameState::COLLISION; }
	private:
		friend class Singleton<GameContext>;
		GameContext() = default;
		SDL_Window* m_pWindow = nullptr;
		GameState m_GameState = GameState::PLAY;
	};
}