#include "pch.h"
#include "SpriteRendererComponent.h"
#include "Engine.h"

namespace sr {

	FACTORY_REGISTER(SpriteRendererComponent);

	void SpriteRendererComponent::Start(){
		if (!m_textureName.empty()) {
			m_texture = Resources().Get<Texture>(m_textureName, Engine::Get().GetRenderer());
			if (m_texture) {
				m_size = m_texture->GetSize();
			}
		}
	}

	void SpriteRendererComponent::Draw(const Renderer& renderer){
		
		if (m_texture) {
			if (m_srcRect.size.w > 0 && m_srcRect.size.h > 0) {
				renderer.DrawTexture(*m_texture, m_srcRect, GetOwner()->GetTransform(), m_flipH);
			}
			else {
				renderer.DrawTexture(*m_texture, GetOwner()->GetTransform(), m_flipH);
			}
			
		}
	}

	void SpriteRendererComponent::Read(const json::value_t& value){
		
		RendererComponent::Read(value);

		JSON_READ_NAME(value, "texture", m_textureName);
		JSON_READ_NAME(value, "flip_h", m_flipH);
		JSON_READ_NAME(value, "origin", m_origin);
	}
}
