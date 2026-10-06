#pragma once
#include "CharacterBase.h"

namespace sr { class PhysicsComponent; class SpriteAnimatorComponent; }

class PlayerController : public CharacterBase {
public:
	CLASS_PROTOTYPE(PlayerController);

	void Start() override;
	void Update(float dt, const float width, const float height) override;

	void OnCollision(sr::Actor* o) override;

	void Read(const sr::json::value_t& value) override;

protected:
	sr::PhysicsComponent* m_physComponent = nullptr;
	sr::SpriteAnimatorComponent* m_rendererComponent = nullptr;
};

