#include "pch.h"
#include "Engine.h"

#include "ModelComponent.h"
#include "Renderer/Uniforms.h"

namespace sr
{
	FACTORY_REGISTER(ModelComponent)

	void ModelComponent::Draw(const Renderer& renderer)
	{
		ObjectUniform ou{
			.model = m_owner->GetTransform().GetMatrix(),
			.normalMatrix = m_owner->GetTransform().GetMatrix()
		};

		renderer.SetVertexUniform(UniformSlot::Object, &ou, sizeof(ou));
		renderer.SetFragmentUniform(UniformSlot::Object, &ou, sizeof(ou));

		m_model->Draw(renderer);
	}

	void ModelComponent::Read(const json::value_t & value)
	{
		RendererComponent::Read(value);

		std::string modelName;
		JSON_READ_NAME(value, "model", modelName);

		m_model = Resources().Get<Model>(modelName, Engine::Get().GetRenderer());
	}
}