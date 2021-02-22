#pragma once
#include "Singleton.h"

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		void SetElapsedTime(float deltaT)
		{
			m_DeltaT = deltaT;
		}

		//get time in milliseconds
		[[nodiscard]] float GetElapsedTime() const // get time in milliseconds
		{
			return m_DeltaT;
		}
	private:
		friend class Singleton<Time>;
		Time() = default;
		float m_DeltaT = 0.f; //in milliseconds
	};
	
}