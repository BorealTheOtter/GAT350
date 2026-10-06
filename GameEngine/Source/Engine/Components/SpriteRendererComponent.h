#pragma once
#include "RendererComponent.h"
#include "Renderer/Texture.h"
#include "Math/Rect.h"

namespace sr {
	class SpriteRendererComponent : public RendererComponent {
	public:
		CLASS_PROTOTYPE(SpriteRendererComponent);

		void Start() override;
		void Draw(const Renderer& renderer) override;
		virtual void Read(const json::value_t& value) override;

		const Vector2& GetSize() const { return m_size; }

		bool GetFlipH() const { return m_flipH; }
		void SetFlipH(bool flipH = false) { m_flipH = flipH; }
	protected:
		std::string m_textureName;
		Rect m_srcRect;
		Vector2 m_size{ 0.0f };
		Vector2 m_origin{ 0.5f };
		bool m_flipH{ false };
		res_t<Texture> m_texture;

	};
}