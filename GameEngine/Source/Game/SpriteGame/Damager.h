#pragma once
#include "Framework/Actor.h"

class Damager : public sr::Actor {
public:
	CLASS_PROTOTYPE(Damager);

	void SetDamage(float damage) { m_damage = damage; }
	float GetDamage() const { return m_damage; }

	void Read(const sr::json::value_t& value) override;

protected:
	float m_damage{ 1.0f };
};