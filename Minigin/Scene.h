#pragma once
#include "SceneManager.h"
#include <backends\imgui_impl_sdl.h>

namespace dae
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<Scene>
	{
		friend std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name);
	public:
		void AddObject(std::shared_ptr<GameObject> object);
		void RemoveObject(std::shared_ptr<GameObject> object);
		const std::string& GetSceneName() { return m_Name; }

		void Update();
		void Render() const;
		void Pause() { m_IsAwake = false; }
		void UnPause() { m_IsAwake = true; }
		bool IsActive() { return m_IsAwake; }

		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		void Add();
		void Remove();
		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectsToDelete{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectsToAdd{};

		static unsigned int m_IdCounter; 

		bool m_IsAwake = true;
	};

}
