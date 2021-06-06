#pragma once
#include "Singleton.h"
#include <backends\imgui_impl_sdl.h>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name);
		std::shared_ptr<Scene> GetScene(const std::string& name);
		void RemoveScene(const std::string& name);
		void LoadScene(const std::string& name);
		void UnloadScene(const std::string& name);

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::vector<std::shared_ptr<Scene>> m_ActiveScenes;
	};
}
