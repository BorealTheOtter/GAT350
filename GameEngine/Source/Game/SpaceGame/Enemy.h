#pragma once
#include "Framework/Actor.h"

struct EnemyDesc : public sr::ActorDesc
{
	float speed = 200.0f;
};
class Enemy : public sr::Actor
{
public:
	Enemy() = default;
	Enemy(EnemyDesc ed) :Actor{ ed }, m_speed{ ed.speed } {};

	CLASS_PROTOTYPE(Enemy);

	void OnCollision(Actor* other) override;

	void SetSpeed(float speed) { m_speed = speed; }

	void Update(float dt, const float width, const float height) override;

	void Read(const sr::json::value_t& value) override;
private:
	float m_speed = 200.0f;
};