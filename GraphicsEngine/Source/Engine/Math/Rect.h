#pragma once
#include "Math/Vector2.h"

namespace sr {
	struct Rect {
		Vector2 pos, size;
		Rect() : pos{ 0 }, size{ 0 } {}
		Rect( float x,float y,float w,float h ) : pos{ x, y }, size{ w,h } {}
	};
}