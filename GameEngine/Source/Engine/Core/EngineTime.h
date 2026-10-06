#pragma once
#include <cstdint>
namespace sr
{
	class Time 
	{
	public:
		Time();

		void Reset();
		void Tick();

		float GetTime() const { return TicksToSeconds(m_currTicks - m_startTicks); }
		float GetDeltaTime() const { return TicksToSeconds(m_deltaTicks) * m_timeScale; }

		float TicksToSeconds(uint64_t ticks) const { return (float)ticks / 1000000000; }

		void SetTimeScale (float timeScale) { m_timeScale = timeScale; }

	private:
		uint64_t m_startTicks = 0;
		uint64_t m_currTicks = 0;
		uint64_t m_prevTicks = 0;
		uint64_t m_deltaTicks = 0;

		float m_timeScale = 1.0f;
	};
}