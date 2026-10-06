#include "pch.h"
#include "RigidBodyComponent.h"

#include "Engine.h"

namespace sr {
	FACTORY_REGISTER(RigidBodyComponent);

	void RigidBodyComponent::Update(float dt){
		m_vel += m_accel * dt;
		m_vel *= (1.0f / (1.0f + m_damping * dt));

		m_angVel += m_angAccel * dt;
		m_angVel *= (1.0f / (1.0f + m_damping * dt));


		GetOwner()->SetPosition(GetOwner()->GetTransform().pos + (m_vel * dt));
		GetOwner()->SetRotation(GetOwner()->GetTransform().rot + (m_angVel * dt));

		m_accel = Vector2{ 0.0f };
		m_angAccel = 0.0f;
	}

	void RigidBodyComponent::ApplyForce(const Vector2& force){
		m_accel += force / m_mass;
	}

	void RigidBodyComponent::SetVelocity(const Vector2 & velocity){
		m_vel = velocity;
	}

	Vector2 RigidBodyComponent::GetVelocity(){
		return m_vel;
	}

	void RigidBodyComponent::ApplyTorque(float torque){
		m_angAccel += torque / m_mass;
	}

	void RigidBodyComponent::SetAngularVelocity(float angularVelocity){
		m_angVel = angularVelocity;
	}

	float RigidBodyComponent::GetAngularVelocity() const
	{
		return m_angVel;
	}

	void RigidBodyComponent::SetPosition(const Vector2& position){
		GetOwner()->SetPosition(position);
	}

	Vector2 RigidBodyComponent::GetPosition() const
	{
		return GetOwner()->GetTransform().pos;
	}

	void RigidBodyComponent::SetRotation(const float& rot)
	{
		GetOwner()->SetRotation(rot);
	}

	float RigidBodyComponent::GetRotation() const
	{
		return GetOwner()->GetTransform().rot;
	}

	void RigidBodyComponent::Read(const json::value_t& value){
		PhysicsComponent::Read(value);

		JSON_READ_NAME(value, "vel", m_vel);
		JSON_READ_NAME(value, "ang_vel", m_angVel);
	}



}