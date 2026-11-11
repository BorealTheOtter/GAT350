#include "pch.h"
#include "Engine.h"


#include "ModelComponent.h"
#include "Renderer/MaterialInstance.h"
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
		if (JSON_READ_NAME(value, "model", modelName)) {
			m_model = Resources().Get<Model>(modelName, Engine::Get().GetRenderer());
		}

		// "materials": [ "materials/brick.instance.json", null ] - one entry per slot, null keeps the model's material
		if (JSON_HAS_NAME(value, "materials"))
		{
			for (const auto& entry : JSON_GET_NAME(value, "materials").GetArray())
			{
				if (entry.IsString())
				{
					auto material = Resources().Get<MaterialInstance>(entry.GetString(), Engine::Get().GetRenderer());
					if (!material) std::cerr << "Could not load material: " << entry.GetString() << std::endl;
					m_materials.push_back(material);
				}
				else
				{
					m_materials.push_back(nullptr);
				}
			}
		}

	}
}