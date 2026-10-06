#include "pch.h"
#include "Engine.h"

namespace sr {
	

	bool Engine::Initialize()
	{
		m_renderer.Initialize("Game Engine", (int)m_screen.x, (int)m_screen.y);
		m_particleSystem.Initialize(5000);
		m_input.Initialize();
		m_time.Reset();
		m_audio.Initialize();
		m_physics.Initialize();

		return true;
	}
	void Engine::Shutdown()
	{
		m_audio.Shutdown();
		m_input.Shutdown();
		m_particleSystem.Shutdown();
		m_renderer.Quit();
		m_physics.Shutdown();
	}
	void Engine::Update()
	{
		m_audio.Update();
		m_particleSystem.Update(m_time.GetDeltaTime());
		m_physics.Update(m_time.GetDeltaTime());
		m_input.Update();
		m_time.Tick();
	}
}