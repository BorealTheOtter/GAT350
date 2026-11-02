#pragma once
#include "RendererComponent.h"

namespace sr
{
	class CameraComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(CameraComponent)

		void Update(float dt) override;
		void Draw(const Renderer& renderer) override;

		virtual void Read(const json::value_t& value) override;

	public:
		float fov{ 70.0f };
		float nearPlane{ 0.001f };
		float farPlane{ 300.0f };
	private:
		glm::mat4x4 m_view;
		glm::mat4x4 m_projection;
	};
}