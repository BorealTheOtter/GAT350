#pragma once
#include "Framework/Actor.h"

struct PlayerDesc : public sr::ActorDesc 
{
	float speed = 200.0f;
};
	class Player : public sr::Actor
	{
	public:
		Player() = default;
		Player(const PlayerDesc& pd) : Actor{ pd }, m_speed{ pd.speed } {};

		CLASS_PROTOTYPE(Player);

		void Update(float dt, const float width, const float height) override;

		void OnCollision(Actor* other) override;

		void Read(const sr::json::value_t& value) override;
	private:
		float m_speed = 200.0f;
	};