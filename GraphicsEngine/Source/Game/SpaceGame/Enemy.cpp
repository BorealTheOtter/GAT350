#include "Enemy.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Engine.h"
#include "Components/PhysicsComponent.h"

void Enemy::OnCollision(Actor* other)
{

    if (other->GetTag() == "PlayerBullet") {
        other->SetDestroyed(); 

        // create particle explosion
        for (int i = 0; i < 100; i++)
        {
            sr::Particle particle;
            particle.position = m_transform.pos;
            particle.color = { sr::RandomFloat(0.9803921569f, 1.0f), sr::RandomFloat(0.1176470588f, 0.9215686275f), 0.1176470588f };
            particle.lifespan = sr::RandomFloat(0.5f, 2.0f);
            particle.velocity = { sr::RandomFloat(-600.0f, 600.0f), sr::RandomFloat(-600.0f, 600.0f) };

            sr::Engine::Get().GetPS().AddParticle(particle);
        }

        sr::Engine::Get().GetAudio().PlaySound("explosion");
        ((SpaceGame*)m_scene->GetGame())->AddPoints(500);
        SetDestroyed();
        
    }

    
}

void Enemy::Update(float dt, const float width, const float height)
{
    Player* player = m_scene->GetActorByTag<Player>("Player");
    if (player) 
    {
        sr::Vector2 direction = player->GetTransform().pos - m_transform.pos;
        float rotation = direction.Angle() * sr::math::RAD_TO_DEG;
        

        auto physicsComponent = GetComponent<sr::PhysicsComponent>();
        if (physicsComponent) {
            sr::Vector2 forward{ 1,0 };
            sr::Vector2 force = forward.Rotate(m_transform.rot * sr::math::DEG_TO_RAD) * m_speed;
            physicsComponent->ApplyForce(force);
            physicsComponent->SetRotation(rotation);

            sr::Vector2 position = physicsComponent->GetPosition();
            position.x = sr::math::Wrap(position.x, 0.0f, width);
            position.y = sr::math::Wrap(position.y, 0.0f, height);
            physicsComponent->SetPosition(position);

        }
    }


    sr::Vector2 vel{ 1,0 };

    sr::Vector2 offset = sr::Vector2{ 1.0f, 0.0f }.Rotate(m_transform.rot * sr::math::DEG_TO_RAD) * -15.0f;

        sr::Particle particle;
        particle.position = m_transform.pos + offset;
        particle.rotation = m_transform.rot + sr::RandomFloat(-20.0f, 20.0f);
        particle.color = { sr::RandomFloat(0.9803921569f, 1.0f), sr::RandomFloat(0.1176470588f, 0.9215686275f), 0.1176470588f };
        particle.lifespan = sr::RandomFloat(0.5f, 1.5f);
        particle.active = true;
        particle.velocity = sr::Vector2{ 1.0f,0.0f }.Rotate(particle.rotation * sr::math::DEG_TO_RAD) * sr::RandomFloat(-200.0f, 0.0f);

        sr::Engine::Get().GetPS().AddParticle(particle);

    //vel = vel.Rotate(m_transform.rot * sr::math::DEG_TO_RAD) * thrust;
    //AddVelocity(vel * dt);
    Actor::Update(dt, width, height);
}

void Enemy::Read(const sr::json::value_t& value)
{
    Actor::Read(value);
    JSON_READ_NAME(value, "speed", m_speed);
}
