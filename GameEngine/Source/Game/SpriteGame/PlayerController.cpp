#include "PlayerController.h"

#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorComponent.h"

#include "SpriteGame.h"

#include "Damager.h"

#include "Engine.h"

FACTORY_REGISTER(PlayerController)

void PlayerController::Start(){
	CharacterBase::Start();

	m_physComponent = GetComponent<sr::PhysicsComponent>();
	assert(m_physComponent);
	m_rendererComponent = GetComponent<sr::SpriteAnimatorComponent>();
	assert(m_rendererComponent);
}

void PlayerController::Update(float dt, const float width, const float height){
	float dir = 0.0f;
	sr::Vector2 vel = m_physComponent->GetVelocity();

	switch (m_state
)
	{
	case CharacterBase::State::Move:
	{
		if (sr::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir = -1.0f;
		if (sr::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir = 1.0f;

		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_W)) {
			vel.y -= 800.0f; //IDK Why negative means up but whatever lol
		}


		if (dir != 0.0f) {
			vel.x = dir * 300.0f;
			m_rendererComponent->Play("run");
			m_rendererComponent->SetFlipH(dir < 0.0f);
		}
		else {
			m_rendererComponent->Play("idle");
		}

		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
			m_state = State::Attack;
			int rand = sr::RandomInt(2);
			if (rand == 0) {
				m_rendererComponent->Play("attack");
			}
			else {
				m_rendererComponent->Play("attack2");
			}
			auto damager = sr::Factory::Instance().Create<sr::Actor>("Proto_Damager");
			damager->SetPosition(GetTransform().pos + sr::Vector2{(m_rendererComponent->GetFlipH()) ? -40.0f : 40.0f, -5.0f});
			damager->SetTag("PlayerDamager");
			m_scene->AddActor(std::move(damager));
			sr::Engine::Get().GetAudio().PlaySound("slash");
		}
		break;
	}
		
		
	case CharacterBase::State::Attack:
	{
		if (m_rendererComponent->IsDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	}
	case CharacterBase::State::Hit:
	{ 
		if (m_rendererComponent->IsDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	}
	case CharacterBase::State::Death:
	{
		if (m_rendererComponent->IsDone()) {
			SetDestroyed();
			((SpriteGame*)m_scene->GetGame())->SetGameState(SpriteGame::GameState::Dead);
		}
		break;
	}
	default:
	{
		break;
	}
	}
	
	m_physComponent->SetVelocity(vel);

	sr::Engine::Get().GetRenderer().SetCamera(m_physComponent->GetPosition());

	CharacterBase::Update(dt, width, height);
}

void PlayerController::OnCollision(sr::Actor * o){
	if (sr::EqualsIgnoreCase(o->GetTag(), "EnemyDamager") && m_state != State::Death) {

		sr::Engine::Get().GetAudio().PlaySound("hit");
		m_state = State::Hit;
		m_rendererComponent->Play("hurt");
		Damager* damager = dynamic_cast<Damager*>(o);
		if (damager) {
			m_health -= damager->GetDamage();
		}
		else {
			m_health -= 1.0f;
		}

		if (m_health <= 0) {
			m_state = State::Death;
			m_rendererComponent->Play("death");
		}
		o->SetDestroyed();
	}
}

void PlayerController::Read(const sr::json::value_t & value){
	CharacterBase::Read(value);
}
