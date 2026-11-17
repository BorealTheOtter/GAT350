#pragma once
#include "Component.h"

namespace sr {
	class CameraController : public Component
	{
	public:
		CLASS_PROTOTYPE(CameraController)

		void Update(float dt) override;

		void Read(const json::value_t& value) override;

		void UpdateGui() override;

	private:
		float m_speed{ 1.0f };
		float m_sens{ 1.0f };

		float m_yaw{ 0.0f };
		float m_pitch{ 0.0f };
	};
}