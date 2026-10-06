#pragma once
#include "SpriteRendererComponent.h"
#include "Resources/ResourceManager.h"
namespace sr{
	class SpriteAnimatorComponent : public SpriteRendererComponent {
	public:
		CLASS_PROTOTYPE(SpriteAnimatorComponent);

		struct SpriteAnimation
		{
			std::string name;
			float fps = 10.0;
			bool loop = true;
			res_t<class TexFrames> texFrames;
		};

		void Start() override;
		void Update(float dt) override;
		void Play(const std::string& name);

		bool GetPause() const { return m_pause; };
		void SetPause(bool pause = false) { m_pause = pause; }

		unsigned int GetFrame() const { return m_frame; }
		bool IsDone() const;

		const std::string& GetAnimationName() const { return m_spriteAnim.name; }

		void Read(const json::value_t& value) override;
	private:
		unsigned int m_frame = 0;
		float m_frameTimer = 0.0f;
		bool m_pause = false;

		std::string m_defaultAnimName;
		SpriteAnimation m_spriteAnim;

		std::map<std::string, SpriteAnimation> m_spriteAnimations;
	};
}