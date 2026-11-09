#include "pch.h"
#include "Framework/Scene.h"
#include "Core/Factory.h"

#include "Renderer/Renderer.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"

namespace sr {
	
	void Scene::AddActor(std::unique_ptr<Actor> actor) {
		actor->m_scene = this;
		m_pendingActors.push_back(std::move(actor));
	}

	void Scene::ClearActors(bool force)
	{
		std::erase_if(m_actors, [force](auto& actor) {return !actor->GetPersistent() || force; });
	}

	void Scene::Update(float dt,float width, float height)
	{
		for (auto& actor : m_actors)
		{
			actor->Update(dt, width, height);
		}

		for (auto& actor : m_actors)
		{
			if (actor->m_destroyed) actor->OnDestroy();
		}

		std::erase_if(m_actors, [](auto& actor) {return actor->GetDestroyed(); });

		for (auto& actor : m_pendingActors) {
			actor->Start();
			m_actors.push_back(std::move(actor));
		}

		m_pendingActors.clear();
	}

	void Scene::Draw(const Renderer& renderer)
	{
		FrameUniform fu{};

		auto camera = GetActorByName<Actor>("Camera");
		if (camera) {
			auto cameraComponent = camera->GetComponent<CameraComponent>();
			if (cameraComponent) {
				fu.camPos = camera->GetTransform().pos;
				fu.view = cameraComponent->GetView();
				fu.projection = cameraComponent->GetProjection();
				fu.viewProjection = fu.projection * fu.view;

				renderer.SetVertexUniform(UniformSlot::Frame, &fu, sizeof(fu));
				renderer.SetFragmentUniform(UniformSlot::Frame, &fu, sizeof(fu));
			}
		}

		LightUniform lu{};
		lu.ambient = glm::vec3{ 0.2 };

		auto lights = GetActorsByTag("Light");
		for (auto light : lights) {
			auto lightComponent = light->GetComponent<LightComponent>();
			if (lightComponent)
			{
				lu.lights[lu.numLights++] = lightComponent->GetLightData();
			}
		}
		renderer.SetVertexUniform(UniformSlot::Light, &lu, sizeof(lu));
		renderer.SetFragmentUniform(UniformSlot::Light, &lu, sizeof(lu));

		for (auto& actor : m_actors)
		{
			actor->Draw(renderer);
		}


	}
	bool Scene::Load(const std::string sceneName)
	{
		sr::json::document_t document;
		if (sr::json::Load(sceneName, document)) {
			if (JSON_HAS_NAME(document, "actors")) {
				for (auto& actorValue : JSON_GET_NAME(document, "actors").GetArray()) {
					std::string typeName;
					JSON_READ_NAME(actorValue, "type", typeName);

					auto actor = sr::Factory::Instance().Create<Actor>(typeName);
					actor->Read(actorValue);

					bool prototype = false;
					JSON_READ(actorValue, prototype);

			

					if (prototype) {
						std::string name;
						JSON_READ(actorValue, name);
						sr::Factory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else {
						AddActor(std::move(actor));
					}
				}
			}
		}
		else {
			return false;
		}
		return true;
	}
}
