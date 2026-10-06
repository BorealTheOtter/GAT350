#include "pch.h"
#include "SpriteAnimationRendererComponent.h"

#include "Engine.h"
#include "Renderer/TexFrames.h"

namespace sr {
	FACTORY_REGISTER(SpriteAnimationRendererComponent);

	void SpriteAnimationRendererComponent::Start()
	{
		if (!m_TexFramesName.empty()) {
			m_texFrames = Resources().Get<TexFrames>(m_TexFramesName, Engine::Get().GetRenderer());
			if (m_texFrames) {
				m_srcRect = m_texFrames->GetFrameRect(0);
				m_size = m_srcRect.size;
				m_texture = m_texFrames->GetTexture();
			}
		}
	}

	void SpriteAnimationRendererComponent::Update(float dt)
	{
		m_frameTimer += dt;
		float frameTime = 1.0f / m_fps;
		while (m_frameTimer >= frameTime) {
			
			m_frame++;

			if (m_loop) { 
				m_frame = m_frame % m_texFrames->GetTotalFrames(); 
			} else {
				if (m_frame >= m_texFrames->GetTotalFrames() - 1) {
					m_frame = m_texFrames->GetTotalFrames() - 1;
				}
			}

			m_frameTimer -= frameTime;
		}

		m_srcRect = m_texFrames->GetFrameRect(m_frame);
	}
	void SpriteAnimationRendererComponent::Read(const json::value_t& value)
	{
		SpriteRendererComponent::Read(value);

		sr::json::Read(value, "fps", m_fps, true);
		JSON_READ_NAME(value, "loop", m_loop);


		JSON_READ_NAME_REQ(value, "tex_frames", m_TexFramesName);


	}
}