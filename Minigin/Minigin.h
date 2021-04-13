#pragma once
struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		void Cleanup();
		void Run();
	protected:
		virtual void LoadGame() const = 0;
	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
	};
}