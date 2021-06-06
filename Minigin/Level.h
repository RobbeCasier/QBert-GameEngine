#pragma once

namespace dae
{
	class Level
	{
	public:
		Level(const std::string& name) : m_Name{ name } {};
		virtual ~Level() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Deactivate() = 0;

		const std::string& GetName() { return m_Name; }
	private:
		std::string m_Name;
	};
}

