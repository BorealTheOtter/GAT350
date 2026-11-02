#include "pch.h"
#include "MaterialComponent.h"
#include "Core/Factory.h"

namespace sr
{
	FACTORY_REGISTER(MaterialComponent)

	void MaterialComponent::Draw(const Renderer& renderer)
	{}

	void MaterialComponent::Read(const json::value_t & value)
	{}
}