#include "pch.h"
#include "SpriteAnimatorComponent.h"
#include "Renderer/TexFrames.h"
#include "Engine.h"

namespace sr {
	FACTORY_REGISTER(SpriteAnimatorComponent);

	void SpriteAnimatorComponent::Start(){
		if (m_defaultAnimName.empty()) {
			m_defaultAnimName = m_spriteAnimations.begin()->first;
		}
	
		Play(m_defaultAnimName);

	}

	void SpriteAnimatorComponent::Update(float dt) {

		m_frameTimer += dt;
		float frameTime = 1.0f / m_spriteAnim.fps;
		while (m_frameTimer >= frameTime) {

			m_frame++;

			if (m_spriteAnim.loop) {
				m_frame = m_frame % m_spriteAnim.texFrames->GetTotalFrames();
			}
			else {
				if (m_frame >= m_spriteAnim.texFrames->GetTotalFrames() - 1) {
					m_frame = m_spriteAnim.texFrames->GetTotalFrames() - 1;
				}
			}

			m_frameTimer -= frameTime;
		}

		m_srcRect = m_spriteAnim.texFrames->GetFrameRect(m_frame);
	}

	void SpriteAnimatorComponent::Play(const std::string& name){
		if (EqualsIgnoreCase(name, m_spriteAnim.name)) return;
		
		auto iter = m_spriteAnimations.find(ToLower(name));
		if (iter == m_spriteAnimations.end()) {
			std::cerr << "could not find animation: " << name << std::endl;
			return;
		}

		m_spriteAnim = iter->second;

		m_frame = 0;
		m_frameTimer = 0.0f;

		m_texture = m_spriteAnim.texFrames->GetTexture();
		m_srcRect = m_spriteAnim.texFrames->GetFrameRect(0);
	}

	bool SpriteAnimatorComponent::IsDone() const
	{
		return(m_frame == m_spriteAnim.texFrames->GetTotalFrames() - 1);
	}

	void SpriteAnimatorComponent::Read(const json::value_t & value){
		SpriteRendererComponent::Read(value);

		JSON_READ_NAME(value, "default_animation", m_defaultAnimName);

		if (JSON_HAS(value, animations) && JSON_GET(value, animations).IsArray())
		{
			for (auto& animatorValue : JSON_GET(value, animations).GetArray())
			{
				SpriteAnimation spriteAnimation;

				std::string textureAnimationName;

				JSON_READ_NAME(animatorValue, "name", spriteAnimation.name);

				std::string tex_frames;
				JSON_READ_REQ(animatorValue, tex_frames);

				if (!tex_frames.empty())
				{
					spriteAnimation.texFrames = Resources().Get<TexFrames>(tex_frames, Engine::Get().GetRenderer());
					if (!spriteAnimation.texFrames)
					{
						std::cerr << "Could not load texture frames: " << tex_frames << std::endl;
					}
				}

				JSON_READ_NAME_REQ(animatorValue, "fps", spriteAnimation.fps);
				JSON_READ_NAME(animatorValue, "loop", spriteAnimation.loop);

				m_spriteAnimations[ToLower(spriteAnimation.name)] = spriteAnimation;
			}
		}

	}
}

