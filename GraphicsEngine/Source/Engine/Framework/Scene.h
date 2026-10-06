#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Framework/Actor.h"


namespace sr {
	
	class Scene {
	public:
		void AddActor(std::unique_ptr<Actor> actor);

		void ClearActors(bool force = false);

		void Update(float dt, float width, float height);
		void Draw(const class Renderer& renderer);

		template<typename T>
		T* GetActorByName(const std::string& name);

		template<typename T>
		T* GetActorByTag(const std::string& tag);

		void SetGame(class Game* game) { m_game = game; }
		class Game* GetGame() { return m_game; }

		bool Load(const std::string sceneName);

	private:
		void UpdateCollisions();
		
	private:
		std::vector<std::unique_ptr<Actor>> m_actors;
		std::vector<std::unique_ptr<Actor>> m_pendingActors;

		class Game* m_game = nullptr;

	};
	
	template<typename T>
	inline T* Scene::GetActorByName(const std::string& name)
	{
		for (auto& actor : m_actors) 
		{
			T* actorT = dynamic_cast<T*>(actor.get());
			if (actorT && actorT->m_name == name) {
				return actorT;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline T* Scene::GetActorByTag(const std::string& tag)
	{
		for (auto& actor : m_actors)
		{
			T* actorT = dynamic_cast<T*>(actor.get());
			if (actorT && actorT->m_tag == tag) {
				return actorT;
			}
		}
		return nullptr;
	}
}