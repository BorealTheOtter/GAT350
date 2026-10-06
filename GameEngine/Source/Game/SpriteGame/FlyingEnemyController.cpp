#include "FlyingEnemyController.h"

#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorComponent.h"

#include "SpriteGame.h"

#include "Damager.h"

#include "Engine.h"

FACTORY_REGISTER(FlyingEnemyController)

void FlyingEnemyController::Start(){
	CharacterBase::Start();

	m_physComponent = GetComponent<sr::PhysicsComponent>();
	assert(m_physComponent);
	m_rendererComponent = GetComponent<sr::SpriteAnimatorComponent>();
	assert(m_rendererComponent);
}

void FlyingEnemyController::Update(float dt, const float width, const float height){
	sr::Vector2 vel = m_physComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{ 
		auto player = m_scene->GetActorByName<Actor>("Proto_Player");
		if (player) {
			sr::Vector2 pos = m_physComponent->GetPosition();
			sr::Vector2 playerPos = player->GetTransform().pos;

			sr::Vector2 dir = playerPos - pos;

			if (dir.Length() < 40.0f) {
				m_state = State::Attack;
				m_rendererComponent->Play("attack");
				auto damager = sr::Factory::Instance().Create<Damager>("Proto_Damager");
				damager->SetDamage(2.0f);
				damager->SetPosition(GetTransform().pos);
				damager->SetTag("EnemyDamager");
				m_scene->AddActor(std::move(damager));
				sr::Engine::Get().GetAudio().PlaySound("shoot");
			}

			m_rendererComponent->SetFlipH(dir.x < 0.0f);
			m_physComponent->ApplyForce(dir.Normalize() * 900.0f);
		}
		break; 
	}
	case CharacterBase::State::Attack:
		if (m_rendererComponent->IsDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
		break;
	case CharacterBase::State::Hit:
	{
		if (m_rendererComponent->IsDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	}
	case CharacterBase::State::Death:
		SetDestroyed();
		break;
	default:
		break;
	}

	CharacterBase::Update(dt, width, height);
}

void FlyingEnemyController::OnCollision(sr::Actor * o){
	if (sr::EqualsIgnoreCase(o->GetTag(), "PlayerDamager")) {

		sr::Engine::Get().GetAudio().PlaySound("hit");
		m_state = State::Hit;
		m_rendererComponent->Play("hit");
		Damager* damager = dynamic_cast<Damager*>(o);
		if (damager) {
			m_health -= damager->GetDamage();
		}
		else {
			m_health -= 1.0f;
		}

		if (m_health <= 0) {
			m_state = State::Death;
			((SpriteGame*)m_scene->GetGame())->AddPoints(500);
		}
		o->SetDestroyed();
	}
}

void FlyingEnemyController::Read(const sr::json::value_t & value){
	CharacterBase::Read(value);
}
