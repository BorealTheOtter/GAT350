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

		if (!m_model) {
			std::cerr << "Could not draw model for actor (" + m_owner->GetName() + ")" << std::endl;
			return;
		}
		ObjectUniform ou{
			.model = m_owner->GetTransform().GetMatrix(),
			.normalMatrix = m_owner->GetTransform().GetMatrix()
		};

		renderer.SetVertexUniform(UniformSlot::Object, &ou, sizeof(ou));
		renderer.SetFragmentUniform(UniformSlot::Object, &ou, sizeof(ou));

		m_model->Draw(renderer, m_materials);
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
	void ModelComponent::UpdateGui()
	{
		ImGui::TextColored({0.0,1.0,0.0,1.0}, "Model");

		if (m_model) ImGui::Text("%s", m_model->GetName().c_str());
		else ImGui::Text("No Model");

		ImGui::TextColored({ 0.0,0.5,1.0,1.0 }, "Materials");

		for (size_t i = 0; i < m_materials.size(); i++)
		{
			ImGui::Text("Material [%d] - %s", i + 1, m_materials[i]->GetName().c_str());
		}
	}
}