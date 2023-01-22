#ifndef VECTOR2_H
#define VECTOR2_H

#include <sdafx.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <yaml-cpp/yaml.h>

#include "../../Math/include/Mathf.h"

namespace RSE
{
	class Vector2
	{
	public:
		float x, y;

	public:
		Vector2(float x = 0, float y = 0) : x(x), y(y)
		{

		}
		Vector2(int x, int y) : x((float)x), y((float)y)
		{

		}
		Vector2(std::initializer_list<float> list) 
		{
			auto it = list.begin();
			x = *it;
			y = *(++it);
		}
		Vector2(std::initializer_list<int> list)
		{
			auto it = list.begin();
			x = (float) *it;
			y = (float) *(++it);
		}

		/*
		Vector2(const Vector2& other) : x(other.x), y(other.y)
		{
			std::cout << "Copied!" << std::endl;
		}
		*/

	public:
		inline Vector2 operator+(const Vector2& other)
		{
			Vector2 vec;
			vec.x = x + other.x;
			vec.y = y + other.y;
			return vec;
		}
		inline Vector2 operator+(const int num)
		{
			Vector2 vec;
			vec.x = x + num;
			vec.y = y + num;
			return vec;
		}
		inline Vector2 operator-(const Vector2& other)
		{
			Vector2 vec;
			vec.x = x - other.x;
			vec.y = y - other.y;
			return vec;
		}
		inline Vector2 operator-(const int num)
		{
			Vector2 vec;
			vec.x = x - num;
			vec.y = y - num;
			return vec;
		}
		inline Vector2 operator*(const Vector2& other)
		{
			Vector2 vec;
			vec.x = x * other.x;
			vec.y = y * other.y;
			return vec;
		}
		inline Vector2 operator*(const int num)
		{
			Vector2 vec;
			vec.x = x * num;
			vec.y = y * num;
			return vec;
		}
		inline Vector2 operator/(const Vector2& other)
		{
			Vector2 vec;
			vec.x = x / other.x;
			vec.y = y / other.y;
			return vec;
		}
		inline Vector2 operator/(const int num)
		{
			Vector2 vec;
			vec.x = x / num;
			vec.y = y / num;
			return vec;
		}
		inline Vector2 operator%(const Vector2& other)
		{
			Vector2 vec;
			if (x != 0 && other.x != 0) vec.x = (float) ((int)x % (int) other.x);
			else vec.x = 0;
			if (y != 0 && other.y != 0) vec.y = (float) ((int)y % (int) other.y);
			else vec.y = 0;
			return vec;
		}
		inline Vector2 operator%(const int num)
		{
			if (num == 0) return Vector2();
			Vector2 vec;
			if (x != 0) vec.x = (float) ((int) x % num);
			else vec.x = 0;
			if (y != 0) vec.y = (float) ((int) y % num);
			else vec.y = 0;
			return vec;
		}
		inline Vector2& operator++()
		{
			x++; y++;
			return *this;
		}
		inline Vector2& operator++(int)
		{
			x++; y++;
			return *this;
		}
		inline Vector2& operator--()
		{
			x--; y--;
			return *this;
		}
		inline Vector2& operator--(int)
		{
			x--; y--;
			return *this;
		}
		inline Vector2& operator+=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		inline Vector2& operator+=(const int num)
		{
			x += num;
			y += num;
			return *this;
		}
		inline Vector2& operator-=(const Vector2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		inline Vector2& operator*=(const Vector2& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		inline Vector2& operator-=(const int num)
		{
			x -= num;
			y -= num;
			return *this;
		}
		inline Vector2& operator/=(int num)
		{
			x /= num;
			y /= num;
			return *this;
		}
		inline Vector2& operator/=(const Vector2& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		inline bool operator==(const Vector2& other) const
		{
			return x == other.x && y == other.y;
		}
		friend std::ostream& operator<<(std::ostream& stream, const Vector2& vec)
		{
			stream << "(" << vec.x << ", " << vec.y << ")";
			return stream;
		}
		inline std::string ToString() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}
		inline bool Equals(const Vector2& vec) const
		{
			return vec.x == x && vec.y == y;
		}
		inline void Zero()
		{
			this->x = 0;
			this->y = 0;
		}
		inline float DistanceTo(const Vector2& other) const
		{
			float dx = Mathf::Abs(other.x - x);
			float dy = Mathf::Abs(other.y - y);
			return Mathf::Sqrt(dx * dx + dy * dy);
			return Mathf::Sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y));
		}
		inline float DistanceToApprox(const Vector2& other) const
		{
			// Calculate using inverse square root which is supposed to be faster?
			// what the fuck is an inverse square root? i dont know
		}
		inline Vector2 Normalize(Vector2 other = { 0, 0 }) const
		{
			float dist = other.DistanceTo(*this);
			return Vector2(x / dist, y / dist);
		}
		inline Vector2& Abs()
		{
			x = Mathf::Abs(x);
			x = Mathf::Abs(y);
			return *this;
		}
		inline Vector2& Lerp(const Vector2& other, float weight)
		{
			x = x + (other.x - x) * weight;
			y = y + (other.y - y) * weight;
			return *this;
		}
		inline Vector2& Clamp(const Vector2& highEnd)
		{
			Vector2 newVec = { Mathf::Clamp(x, 0.0f, highEnd.x), Mathf::Clamp(y, 0.0f, highEnd.y) };
			return newVec;
		}
		inline Vector2& Clamp(const Vector2& lowEnd, const Vector2& highEnd)
		{
			Vector2 newVec = { Mathf::Clamp(x, lowEnd.x, highEnd.x), Mathf::Clamp(y, lowEnd.y, highEnd.y) };
			return newVec;
		}
	};
}

namespace std
{
	template <>
	struct hash<RSE::Vector2>
	{
		// Implement hasing function for Vector2 so that I can use it in STL containers
		size_t operator()(const RSE::Vector2& k) const
		{
			return hash<float>()(k.x) ^ hash<float>()(k.y);
		}
	};
	template <> struct formatter<RSE::Vector2>
	{ 
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const RSE::Vector2& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
		}
	};
}

#endif // VECTOR2_H