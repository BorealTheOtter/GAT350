#pragma once

#include <cmath>
#include <cassert>

namespace sr
{
	struct Vector2 {
		union
		{
			struct { float x, y; };
			struct { float w, h; };
		};

		Vector2() = default;
		Vector2(float x, float y) : x{ x }, y{ y } {}
		Vector2(float v) : x{ v }, y{ v } {}


		float operator[](unsigned int i) const { assert(i < 2); return (&x)[i]; }
		float& operator[](unsigned int i) { assert(i < 2); return (&x)[i]; }

		bool operator == (const Vector2& o) const { return ((this->x == o.x) && (this->y == o.y)); }
		bool operator != (const Vector2& o) const { return ((this->x != o.x) || (this->y != o.y)); }

		Vector2 operator + (const Vector2& v) const { return Vector2{ this->x + v.x, this->y + v.y }; }
		Vector2 operator - (const Vector2& v) const { return Vector2{ this->x - v.x, this->y - v.y }; }
		Vector2 operator * (const Vector2& v) const { return Vector2{ this->x * v.x, this->y * v.y }; }
		Vector2 operator / (const Vector2& v) const { return Vector2{ this->x / v.x, this->y / v.y }; }

		Vector2 operator + (float s) const { return Vector2{ this->x + s, this->y + s }; }
		Vector2 operator - (float s) const { return Vector2{ this->x - s, this->y - s }; }
		Vector2 operator * (float s) const { return Vector2{ this->x * s, this->y * s }; }
		Vector2 operator / (float s) const { return Vector2{ this->x / s, this->y / s }; }

		Vector2 operator += (const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
		Vector2 operator -= (const Vector2& v) { this->x -= v.x; this->y -= v.y; return *this; }
		Vector2 operator *= (const Vector2& v) { this->x *= v.x; this->y *= v.y; return *this; }
		Vector2 operator /= (const Vector2& v) { this->x /= v.x; this->y /= v.y; return *this; }

		Vector2 operator += (float s) { this->x += s; this->y += s; return *this; }
		Vector2 operator -= (float s) { this->x -= s; this->y -= s; return *this; }
		Vector2 operator *= (float s) { this->x *= s; this->y *= s; return *this; }
		Vector2 operator /= (float s) { this->x /= s; this->y /= s; return *this; }

		float LengthSqr() const { return (float)(pow(x, 2) + pow(y, 2)); }
		float Length() const { return sqrt(LengthSqr()); }
		float Distance(const Vector2 v) const { return (v - *this).Length(); }
		Vector2 Normalize() const { return (*this) / Length(); }
		float Dot(const Vector2& v) const { return (this->x * v.x) + (this->y * v.y); }
		float Angle() const { return atan2(this->y, this->x); }
		float AngleBetween(const Vector2& v) const{ return acos(Dot(v)); }
		Vector2 Rotate(float radians) 
		{
			Vector2 v;
			v.x = cos(radians);
			v.y = sin(radians);

			float x = this->x * v.x - this->y * v.y;
			float y = this->x * v.y + this->y * v.x;

			return Vector2{ x, y };
		}
	};
}