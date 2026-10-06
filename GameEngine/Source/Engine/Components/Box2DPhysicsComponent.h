#pragma once
#include "PhysicsComponent.h"
#include "Physics/PhysicsBody.h"

namespace sr {
	class Box2DPhysicsComponent : public PhysicsComponent {
	public:
		CLASS_PROTOTYPE(Box2DPhysicsComponent);

		Box2DPhysicsComponent() = default;
		Box2DPhysicsComponent(const Box2DPhysicsComponent& other);

		void Start() override;
		void Update(float dt) override;

		void ApplyForce(const Vector2& force) override;

		void SetVelocity(const Vector2& velocity) override;
		Vector2 GetVelocity() override;

		void ApplyTorque(float torque) override;

		void SetAngularVelocity(float angularVelocity) override;
		float GetAngularVelocity() const override;

		void SetPosition(const Vector2& pos) override;
		Vector2 GetPosition() const override;

		void SetRotation(const float& rot) override;
		float GetRotation() const override;

		void Read(const json::value_t& value) override;
	private:
		Vector2 m_size{ 0.0f };
		Vector2 m_scale{ 1.0f };

		PhysicsBody::PhysicsBodyDef m_bodyDef;
		std::unique_ptr<PhysicsBody> m_physicsBody;
	};
}