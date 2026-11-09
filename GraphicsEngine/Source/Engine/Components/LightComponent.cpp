#include "pch.h"
#include "LightComponent.h"
#include "Core/Factory.h"
#include "Framework/Actor.h"

namespace sr
{
	FACTORY_REGISTER(LightComponent)

	void LightComponent::Draw(const Renderer& renderer)
	{}
	void LightComponent::Read(const json::value_t & value)
	{
		RendererComponent::Read(value);

		std::string typeName;
		JSON_READ_NAME(value, "light_type", typeName);
		if (typeName == "directional") m_type = LightType::Directional;
		else if (typeName == "spot") m_type = LightType::Spot;
		else m_type = LightType::Point;

		JSON_READ_NAME(value, "color", m_color);
		JSON_READ_NAME(value, "range", m_range);
		JSON_READ_NAME(value, "inner_cutoff", m_innerCutoff);
		JSON_READ_NAME(value, "outer_cutoff", m_outerCutoff);
	}
	Light LightComponent::GetLightData() const
	{
		const Transform& transform = m_owner->GetTransform();
		Light ld = {
			.pos = transform.pos,
			.type = m_type,
			.dir = transform.Forward(),
			.range = m_range,
			.color = m_color,
			.innerCutoff = glm::cos(glm::radians(m_innerCutoff)),
			.outerCutoff = glm::cos(glm::radians(m_outerCutoff)),
		};
		return ld;
	}
}