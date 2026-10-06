#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/MathUtils.h"

#include <vector>

namespace sr {
	struct Particle
	{
		bool active{ false };
		float lifespan{ 1 };

		Vector2 position{ 0, 0 };
		float rotation{0};
		Vector2 velocity{ 0, 0 };
		Color color{ 0, 0, 0 };
	};

	class ParticleSystem
	{
	public:
		ParticleSystem() = default;

		bool Initialize(size_t poolSize = 1000);
		void Shutdown();

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		void AddParticle(const Particle& particle);

	private:
		Particle* GetFreeParticle();

	private:
		std::vector<Particle> m_particles;
	};

	
}