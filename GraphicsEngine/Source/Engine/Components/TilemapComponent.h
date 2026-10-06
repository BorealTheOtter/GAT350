#pragma once
#include "RendererComponent.h"
#include "Resources/ResourceManager.h"

namespace sr {
	class TilemapComponent : public RendererComponent {
	public:
		CLASS_PROTOTYPE(TilemapComponent);

		TilemapComponent() = default;
		TilemapComponent(const TilemapComponent& o);

		void Start() override;
		void Draw(const Renderer& renderer) override;

		void Read(const json::value_t& value) override;
	private:
		std::string m_tilemapName;
		res_t<class Tilemap> m_tilemap;
		std::vector<std::unique_ptr<class PhysicsBody>> m_physicsBodies;
	};
}