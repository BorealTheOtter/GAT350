#include "pch.h"
#include "Core/EngineTime.h"

namespace sr {
	Time::Time()
	{
		Reset();
	}

	void Time::Reset()
	{
		m_startTicks = SDL_GetTicksNS();
		m_prevTicks = SDL_GetTicksNS();
		m_currTicks = SDL_GetTicksNS();
	}

	void Time::Tick()
	{
		m_currTicks = SDL_GetTicksNS();
		m_deltaTicks = m_currTicks - m_prevTicks;
		m_prevTicks = m_currTicks;
	}
}


