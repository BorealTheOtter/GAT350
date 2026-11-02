#include "pch.h"
#include "LightComponent.h"
#include "Core/Factory.h"

namespace sr
{
	FACTORY_REGISTER(LightComponent)

	void LightComponent::Draw(const Renderer& renderer)
	{}
	void LightComponent::Read(const json::value_t & value)
	{}
}