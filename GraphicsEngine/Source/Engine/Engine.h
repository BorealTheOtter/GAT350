#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Core/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include "Core/StringUtils.h"

#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Core/EngineTime.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Core/File.h"
#include "Audio/Audio.h"
#include "Renderer/Text.h"
#include "Renderer/Texture.h"
#include "Renderer/ParticleSystem.h"
#include "Resources/ResourceManager.h"
#include "Serialization/Json.h"
#include "Core/Factory.h"
#include "Physics/Physics.h"

#include "Framework/Game.h"

#include <iostream>
#include <vector>

namespace sr {
	class Engine {
	public:
		static Engine& Get() { static Engine engine{ Vector2{1920,1080} }; return engine; }

		bool Initialize();
		void Shutdown();

		void Update();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		Time& GetTime() { return m_time; }
		Audio& GetAudio() { return m_audio; }
		ParticleSystem& GetPS() { return m_particleSystem; }
		Physics& GetPhysics() { return m_physics; }

		void SetScreen(const Vector2& v) { m_screen = v; }
		const Vector2& GetScreen() const { return m_screen; }

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
	private:
		Engine(Vector2 v) : m_screen{ v } {};
		Engine() = default;
	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		ParticleSystem m_particleSystem;
		Physics m_physics;

		Time m_time;
		Vector2 m_screen{ 640,480 };

	};
}