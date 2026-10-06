#pragma once
#include "SpriteRendererComponent.h"

namespace sr {
	class SpriteAnimationRendererComponent : public SpriteRendererComponent {
	public:
		CLASS_PROTOTYPE(SpriteAnimationRendererComponent);

		void Start() override;
		void Update(float dt) override;
		void Read(const json::value_t& value) override;

	private:
		float m_frameTimer = 0;
		bool m_loop = true;

		float m_fps = 0;
		unsigned int m_frame;

		res_t<class TexFrames> m_texFrames;
		std::string m_TexFramesName;
	};
}