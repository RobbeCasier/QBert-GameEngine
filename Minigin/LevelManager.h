#pragma once
#include "Singleton.h"

namespace dae
{
	class Level;
	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		void AddLevel(std::shared_ptr<Level> level);
		void SetActiveLevel(const std::string& levelName);
		
		void Update();
	private:
		friend class Singleton<LevelManager>;
		LevelManager() = default;
		std::vector<std::shared_ptr<Level>> m_Levels;
		std::shared_ptr<Level> m_pActiveLevel;
	};
}

