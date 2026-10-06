#pragma once
#include "Resources/Resource.h"
#include <string>
#include "Renderer/Renderer.h"
#include "Math/Rect.h"

namespace sr {
	class TexFrames : public Resource {
	public:
		bool Load(const std::string& filename, class Renderer& renderer);
		Rect GetFrameRect(unsigned int frame);

		unsigned int GetTotalFrames() const { return m_totalFrames; }
		res_t<class Texture> GetTexture() const { return m_texture; }
	private:
		unsigned int m_cols = 0;
		unsigned int m_rows = 0;
		unsigned int m_startFrame = 0;
		unsigned int m_totalFrames = 0;

		Vector2 m_cellSize{ 0.0f,0.0f };
		res_t<class Texture> m_texture;
	};
}