#pragma once

#include <string>
#include <memory>

#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Resources/Resource.h"

#include "Components/Component.h"

#include "Object.h"



namespace sr
{
	class Scene;
	class Texture;

	struct ActorDesc 
	{
		Transform transform{ Vector2{0, 0}, 0.0, 1.0 };
		res_t<Model> model;
		res_t<Texture> texture;
		std::string name;
		std::string tag;
		float lifespan{ -1.0f };
	};

	class Actor : public Object
	{
	public:
		Actor() = default;
		Actor(const ActorDesc& ad) : 
			m_transform{ ad.transform }, 
			m_model{ ad.model }, 
			m_tag{ ad.tag }, 
			m_lifespan{ ad.lifespan }, 
			m_texture{ ad.texture } {};

		Actor(const Actor& other);

		CLASS_PROTOTYPE(Actor);

		virtual void Update(float dt, const float width, const float height);
		virtual void Draw(const class Renderer& renderer) const;

		virtual void Start();
		virtual void OnDestroy(); 

		virtual void OnCollision(Actor* other) {}

		const Transform& GetTransform() const { return m_transform; }
		void SetTransform(const sr::Transform& transform) { m_transform = transform; }

		void SetPosition(const Vector2& pos) { m_transform.pos = pos; }
		void SetRotation(float rotation) { m_transform.rot = rotation; }
		void SetScale(float scale) { m_transform.scale = scale; }
		
		const std::string& GetName() const { return m_name; }
		const std::string& GetTag() const { return m_tag; }

		void SetTag(std::string tag) { m_tag = tag; }

		Scene* GetScene() { return m_scene; }

		float GetRadius() const;

		void SetDestroyed(bool destroy = true) { m_destroyed = destroy; }
		bool GetDestroyed() const { return m_destroyed; }

		bool GetPersistent() const { return m_persistent; }

		virtual void Read(const json::value_t& value) override;

		void AddComponent(res_t<Component> component);

		template<std::derived_from<Component> T>
		T* GetComponent();

		friend Scene;

	protected:
		std::string m_tag;

		Transform m_transform;
		float m_lifespan = -1.0f;

		bool m_destroyed = false;
		bool m_persistent = false;

		res_t<Model> m_model;
		res_t<Texture> m_texture;

		std::vector<res_t<Component>> m_components;

		Scene* m_scene = nullptr;
	};

	template<std::derived_from<Component> T>
	inline T* Actor::GetComponent()
	{
		for (auto component : m_components) {
			auto result = dynamic_cast<T*>(component.get());
			if (result) {
				return result;
			}
		}
		return nullptr;
	}
}