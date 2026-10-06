#pragma once
#include "Math/Vector2.h"
#include "Serialization/Json.h"

namespace sr
{
	struct Transform
	{
		Vector2 pos{ 0.0f,0.0f };
		float rot{ 0.0f };
		float scale{ 1.0f };

		void Read(const json::value_t& value) {
			JSON_READ_REQ(value, pos);
			JSON_READ_REQ(value, rot);
			JSON_READ_REQ(value, scale);

		}
	};
}