#include "pch.h"
#include "CameraComponent.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"
#include "Engine.h"

namespace sr
{
	FACTORY_REGISTER(CameraComponent)

		void CameraComponent::Update(float dt)
	{
		m_view = glm::lookAtRH(GetOwner()->GetTransform().pos, GetOwner()->GetTransform().pos + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });

		float aspectRatio = (float)Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
		m_projection = glm::perspectiveRH(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

	void CameraComponent::Draw(const Renderer& renderer)
	{
	
	}

	void CameraComponent::Read(const json::value_t & value)
	{
		RendererComponent::Read(value);
		JSON_READ(value, fov);
		JSON_READ(value, nearPlane);
		JSON_READ(value, farPlane);
	}
}