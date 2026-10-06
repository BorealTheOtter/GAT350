#pragma once
#include <cassert>

namespace sr 
{
	struct Vector3 {
		
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		Vector3() = default;
		Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
		Vector3(float v) : x{ v }, y{ v }, z{ v } {}

		float operator[](unsigned int i) const { assert(i < 3); return (&x)[i]; }
		float& operator[](unsigned int i) { assert(i < 3); return (&x)[i]; }

		bool operator == (const Vector3& o) const { return ((this->x == o.x) && (this->y == o.y) && (this->z == o.z)); }
		bool operator != (const Vector3& o) const { return ((this->x != o.x) || (this->y != o.y) || (this->z != o.z)); }

		Vector3 operator + (const Vector3& v) const { return Vector3{ this->x + v.x, this->y + v.y, this->z + v.z }; }
		Vector3 operator - (const Vector3& v) const { return Vector3{ this->x - v.x, this->y - v.y, this->z - v.z }; }
		Vector3 operator * (const Vector3& v) const { return Vector3{ this->x * v.x, this->y * v.y, this->z * v.z }; }
		Vector3 operator / (const Vector3& v) const { return Vector3{ this->x / v.x, this->y / v.y, this->z / v.z }; }

		Vector3 operator + (float s) const { return Vector3{ this->x + s, this->y + s, this->z + s }; }
		Vector3 operator - (float s) const { return Vector3{ this->x - s, this->y - s, this->z - s }; }
		Vector3 operator * (float s) const { return Vector3{ this->x * s, this->y * s, this->z * s }; }
		Vector3 operator / (float s) const { return Vector3{ this->x / s, this->y / s, this->z / s }; }


		Vector3 operator += (const Vector3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
		Vector3 operator -= (const Vector3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
		Vector3 operator *= (const Vector3& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; return *this; }
		Vector3 operator /= (const Vector3& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; return *this; }


		Vector3 operator += (float s) { this->x += s; this->y += s; this->z += s; return *this; }
		Vector3 operator -= (float s) { this->x -= s; this->y -= s; this->z -= s; return *this; }
		Vector3 operator *= (float s) { this->x *= s; this->y *= s; this->z *= s; return *this; }
		Vector3 operator /= (float s) { this->x /= s; this->y /= s; this->z /= s; return *this; }

		float LengthSqr() const { return (float)(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
		float Length() const { return (float)sqrt(LengthSqr()); }
		Vector3 Normalize() const { return (*this) / Length(); }
		float Dot(const Vector3& v) const { return (this->x * v.x) + (this->y * v.y) + (this->z * v.z); }
	};
	using Color = Vector3;
}