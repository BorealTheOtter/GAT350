#include "pch.h"
#include "Framework/Scene.h"
#include "Core/Factory.h"

#include "Components/ColliderComponent.h"

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


	void Scene::UpdateCollisions()
	{
		for (auto& actorA : m_actors) {
			for (auto& actorB : m_actors) {
				if (actorA == actorB || actorA->m_destroyed || actorB->m_destroyed) continue;

				auto colliderA = actorA->GetComponent<ColliderComponent>();
				auto colliderB = actorB->GetComponent<ColliderComponent>();

				if (!colliderA || !colliderB) continue;

				if (colliderA->CheckCollision(*colliderB)) {
					actorA->OnCollision(actorB.get());
					actorB->OnCollision(actorA.get());
				}
			}
		}
	}
}
