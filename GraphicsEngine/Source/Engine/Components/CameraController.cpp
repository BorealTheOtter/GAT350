#include "pch.h"
#include "CameraController.h"
#include "Engine.h"

namespace sr {
    FACTORY_REGISTER(CameraController)

	void CameraController::Update(float dt)
	{
        //glm::vec3 dir{ 0 };
        ////glm::vec3 rot{ 0 };

        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) dir.y += m_speed;
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir.x -= m_speed;
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) dir.y -= m_speed;
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir.x += m_speed;
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) dir.z -= m_speed;
        //if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) dir.z += m_speed;

        //m_owner->SetPosition(m_owner->GetTransform().pos + (dir * dt));

            // get camera rotation
        if (Engine::Get().GetInput().GetMouseDown(Input::MouseButton::RIGHT))
        {
            glm::vec2 axis = Engine::Get().GetInput().GetMouseDelta() * m_sens;

            m_yaw -= axis.x;
            m_pitch -= axis.y;
            m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
        }

        //owner->transform.SetRotation(glm::vec3(pitch, yaw, 0.0f));

        //glm::quat q = glm::quat_cast(glm::eulerAngleYXZ(glm::radians(m_yaw), glm::radians(m_pitch), 0.0f));
        //m_owner->GetTransform().SetRotationQuat(q);

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction -= m_speed * m_owner->GetTransform().Right();
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction += m_speed * m_owner->GetTransform().Right();
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction += m_speed * m_owner->GetTransform().Up();
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction -= m_speed * m_owner->GetTransform().Up();
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction += m_speed * m_owner->GetTransform().Forward();
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction -= m_speed * m_owner->GetTransform().Forward();;

        if (glm::length2(direction) > 0.0f) {
            direction = glm::normalize(direction);
        }
        m_owner->SetPosition(m_owner->GetTransform().pos + (direction * dt));

	}
    void CameraController::Read(const json::value_t& value)
    {
        Component::Read(value);
        JSON_READ_NAME(value, "speed", m_speed);
    }
}