#pragma once
#include "Component.h"

namespace sr {
	class ColliderComponent : public Component {
	public:
		virtual bool CheckCollision(const ColliderComponent& o) = 0;
	};
}