#pragma once
#include "Component.h"

namespace sr {
	class RendererComponent : public Component{
	public:
		virtual void Draw(const class Renderer& renderer) = 0;
	};
}