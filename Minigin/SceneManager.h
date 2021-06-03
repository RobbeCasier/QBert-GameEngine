#pragma once
#include "Singleton.h"
#include <backends\imgui_impl_sdl.h>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void AddScene(Scene* scene);
		void SetActive(const std::string& sceneName);

		void Update();
		void Render(SDL_Window* window);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_ActiveScene;
	};
}
