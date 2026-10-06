#pragma once
#include "PhysicsComponent.h"

namespace sr {
	class RigidBodyComponent : public PhysicsComponent {
	public:
		CLASS_PROTOTYPE(RigidBodyComponent);

		void Update(float dt) override;

		void ApplyForce(const Vector2& force) override;
		void SetVelocity(const Vector2& velocity) override;
		Vector2 GetVelocity() override;

		void ApplyTorque(float torque) override;
		void SetAngularVelocity(float angularVelocity) override;
		float GetAngularVelocity() const override;

		void SetPosition(const Vector2& position) override;
		Vector2 GetPosition() const override;

		void SetRotation(const float& rot) override;
		float GetRotation() const override;

		void Read(const json::value_t& value) override;
	private:
		Vector2 m_vel{ 0.0f };
		Vector2 m_accel{ 0.0f };
		
		float m_angVel = 0.0f;
		float m_angAccel = 0.0f;


	};
}