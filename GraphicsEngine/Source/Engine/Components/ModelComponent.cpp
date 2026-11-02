#include "pch.h"
#include "ModelComponent.h"
#include "Core/Factory.h"

namespace sr
{
	FACTORY_REGISTER(ModelComponent)

	void ModelComponent::Draw(const Renderer& renderer)
	{}

	void ModelComponent::Read(const json::value_t & value)
	{}
}