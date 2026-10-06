#include "pch.h"
#include "Framework/Actor.h"
#include "Renderer/Renderer.h"
#include "Math/MathUtils.h"
#include "Renderer/Texture.h"
#include "Engine.h"

#include "Components/RendererComponent.h"


namespace sr
{
	FACTORY_REGISTER(Actor);

	Actor::Actor(const Actor& other):
		Object{other},
		m_tag{other.m_tag},
		m_transform{other.m_transform},
		//m_damping{other.m_damping},
		m_lifespan{other.m_lifespan}
	{
		for (const auto& component : other.m_components) {
			auto clone = std::unique_ptr<Component>(dynamic_cast<Component*>(component->Clone().release()));
			AddComponent(std::move(clone));
		}
	}

	void Actor::Update(float dt, const float width, const float height)
	{
		if (m_lifespan != -1.0f) {
			if (m_destroyed) {
				m_lifespan = 0.0f;
			}
			m_lifespan -= dt;
			m_destroyed = (m_lifespan <= 0.0f);
		}

		for (auto component : m_components) {
			if (component->IsActive()) {
				component->Update(dt);
			}
		}

		//m_transform.pos += (m_velocity * dt);
		//m_velocity *= (1.0f / (1.0f + m_damping * dt));
	}

	void Actor::Start() {
		for (auto& c : m_components) {
			c->Start();
		}
	}

	void Actor::OnDestroy() {
		for (auto& c : m_components) {
			c->Destroy();
		}
	}

	void Actor::Draw(const Renderer& renderer) const {

		for (auto component : m_components) {
			//check if component is renderer component
			auto rendererComponent = dynamic_cast<RendererComponent*>(component.get());
			if (rendererComponent && rendererComponent->IsActive()) {
				//draw
				rendererComponent->Draw(renderer);
			}
		}
		
		if (m_model) {
			renderer.DrawModel(*m_model, m_transform);
		}
		if (m_texture) {
			renderer.DrawTexture(*m_texture, m_transform);
		}

	}
	float Actor::GetRadius() const
	{
			if(m_model) return m_model.get()->GetRadius() * m_transform.scale * 0.6f;
			if (m_texture) return (m_texture->GetSize().Length() * 0.5f) * 0.6f;

			return 0.0f;
	}
	void Actor::Read(const json::value_t& value)
	{
		Object::Read(value);
		
		
		JSON_READ_NAME(value, "tag", m_tag);
		JSON_READ_NAME(value, "lifespan", m_lifespan);
		JSON_READ_NAME(value, "persistent", m_persistent);


		if (JSON_HAS_NAME(value, "transform")) {
		m_transform.Read(JSON_GET_NAME(value, "transform"));
		}

		if (JSON_HAS_NAME(value, "components")) {
			for (auto& componentValue : JSON_GET_NAME(value, "components").GetArray()) {
				std::string typeName;
				sr::json::Read(componentValue, "type", typeName);
				if (typeName.empty()) {
					std::cerr << "Error registering component, typeName is null" << std::endl;
					continue;
				}
				
				auto component = Factory::Instance().Create<Component>(typeName);

				if (component) {
					component->Read(componentValue);
					AddComponent(std::move(component));
				}
			}
		}

	}
	void Actor::AddComponent(res_t<Component> component)
	{
		component->SetOwner(this);
		m_components.push_back(std::move(component));
	}
}