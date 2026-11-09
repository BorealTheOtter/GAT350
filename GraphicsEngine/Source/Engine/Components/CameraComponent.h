#pragma once
#include "RendererComponent.h"

namespace sr
{
	class CameraComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(CameraComponent)

		void Start() override;
		void Update(float dt) override;
		void Draw(const Renderer& renderer) override;

		virtual void Read(const json::value_t& value) override;

		const glm::mat4x4& GetView() const { return m_view; }
		const glm::mat4x4& GetProjection() const { return m_projection; }

	public:
		float aspectRatio{(16.0f / 9.0f)};
		float fov{ 70.0f };
		float nearPlane{ 0.001f };
		float farPlane{ 300.0f };
	private:
		glm::mat4x4 m_view;
		glm::mat4x4 m_projection;
	};
}