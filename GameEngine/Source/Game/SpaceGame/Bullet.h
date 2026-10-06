#pragma once
#include "Framework/Actor.h"

struct BulletDesc : public sr::ActorDesc
{
	float speed = 200.0f;
};
class Bullet : public sr::Actor
{
public:
	Bullet() = default;
	Bullet(BulletDesc bd) :Actor{ bd }, m_speed{ bd.speed } {};

	CLASS_PROTOTYPE(Bullet);

	void Update(float dt, const float width, const float height) override;

	void Read(const sr::json::value_t& value) override;
private:
	float m_speed = 200.0f;
};
