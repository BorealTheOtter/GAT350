#pragma once
#include "Component.h"

namespace sr {
	class CameraController : public Component
	{
	public:
		CLASS_PROTOTYPE(CameraController)

		void Update(float dt) override;

		void Read(const json::value_t& value) override;

	private:
		float m_speed{ 1 };
		float m_sens{ 1 };

		float m_yaw{ 0 };
		float m_pitch{ 0 };
	};
}