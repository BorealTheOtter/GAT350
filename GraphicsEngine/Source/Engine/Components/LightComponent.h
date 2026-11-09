#pragma once
#include "RendererComponent.h"
#include "Renderer/Light.h"

namespace sr
{
	class LightComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(LightComponent)

		void Draw(const Renderer& renderer) override;

		virtual void Read(const json::value_t& value) override;

		Light GetLightData() const;

	private:
		LightType m_type;
		float m_range;
		glm::vec4 m_color;
		float m_innerCutoff;
		float m_outerCutoff;
	};
}