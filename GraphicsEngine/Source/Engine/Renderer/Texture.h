#pragma once
#include "Math/Vector2.h"
#include "Resources/Resource.h"

#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

struct SDL_Texture;

namespace sr {
	class Texture : public Resource
	{
	public:
		Texture() = default;
		~Texture();

		bool Load(const std::string& filename, class Renderer& renderer);

		const Vector2& GetSize() const { return m_size; }



	private:
		friend class Renderer;
		SDL_Texture* m_texture{ nullptr };
		Vector2 m_size{ 0,0 };
	};
}