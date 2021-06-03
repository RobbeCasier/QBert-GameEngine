#pragma once
#include "SceneManager.h"
#include <backends\imgui_impl_sdl.h>

namespace dae
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		void Add(const std::shared_ptr<GameObject>& object);
		void RemoveObject(std::shared_ptr<GameObject> object);
		const std::string& GetSceneName() { return m_Name; }

		void RootInitialize();
		void RootUpdate();
		void RootRender(SDL_Window* window) const;

		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		Scene(const std::string& name);
	protected:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Render(SDL_Window* window) const = 0;

	private: 
		void Remove();
		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectsToDelete{};

		static unsigned int m_IdCounter; 
	};

}
