#pragma once
#include "Core/Factory.h"
#include "ColliderComponent.h"

namespace sr {
	class CircleColliderComponent : public ColliderComponent {
	public:
		CLASS_PROTOTYPE(CircleColliderComponent);

		bool CheckCollision(const ColliderComponent& o) override;

		void Read(const json::value_t& value) override;

		float GetRadius() const { return m_radius; }
	private:
		float m_radius = 0.0f;
	};
}