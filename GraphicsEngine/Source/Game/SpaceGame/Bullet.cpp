#include "Bullet.h"
#include "Engine.h"
#include "Components/PhysicsComponent.h"

void Bullet::Update(float dt, const float width, const float height)
{

	auto physicsComponent = GetComponent<sr::PhysicsComponent>();
	if (physicsComponent) {
		sr::Vector2 forward{ 1,0 };
		sr::Vector2 velocity = forward.Rotate(m_transform.rot * sr::math::DEG_TO_RAD) * m_speed;
		physicsComponent->SetVelocity(velocity);

		sr::Vector2 position = physicsComponent->GetPosition();
		position.x = sr::math::Wrap(position.x, 0.0f, width);
		position.y = sr::math::Wrap(position.y, 0.0f, height);
		physicsComponent->SetPosition(position);
	}

	Actor::Update(dt, width, height);
}

void Bullet::Read(const sr::json::value_t& value)
	{
		Actor::Read(value);
		JSON_READ_NAME(value, "speed", m_speed);
	}
