#ifndef VECTOR2_H
#define VECTOR2_H

#include <sdafx.h>

#include "../../Math/include/Mathf.h"

namespace Advres::RSE
{
	class Vector2
	{
	public:
		float x, y;

	public:
		Vector2(float x = 0, float y = 0) : x(x), y(y)
		{

		}
		Vector2(int x, int y) : x((float) x), y((float) y)
		{

		}

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
		inline Vector2 operator+(const float num)
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
		inline Vector2 operator-(const float num)
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
		inline Vector2 operator*(const float num)
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
		inline Vector2 operator/(const float num)
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
		inline Vector2& operator+=(const float num)
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
		inline Vector2& operator-=(const float num)
		{
			x -= num;
			y -= num;
			return *this;
		}
		inline Vector2& operator/=(const int num)
		{
			x /= num;
			y /= num;
			return *this;
		}
		inline Vector2& operator/=(const float num)
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

	public:
		static inline float DistanceTo(const Vector2& vecA, const Vector2& vecB)
		{
			float dx = Mathf::Abs(vecB.x - vecA.x);
			float dy = Mathf::Abs(vecB.y - vecA.y);
			return Mathf::Sqrt(dx * dx + dy * dy);
		}
		static inline float DistanceToApprox(const Vector2& other)
		{
			// Calculate using inverse square root which is supposed to be faster?
			// what the fuck is an inverse square root? i dont know
		}
		static inline Vector2 Normalize(const Vector2& vecA, const Vector2& vecB = { 0, 0 })
		{
			float dist = DistanceTo(vecA, vecB);
			return Vector2(vecA.x / dist, vecB.y / dist);
		}
		static inline Vector2 Abs(const Vector2& vec)
		{
			Vector2 vecB;
			vecB.x = Mathf::Abs(vec.x);
			vecB.y = Mathf::Abs(vec.y);
			return vecB;
		}
		static inline Vector2 Lerp(const Vector2& vecA, const Vector2& vecB, float weight)
		{
			Vector2 vec;
			vec.x = vecA.x + (vecB.x - vecA.x) * weight;
			vec.y = vecA.y + (vecB.y - vecA.y) * weight;
			return vec;
		}
		static inline Vector2 Clamp(const Vector2& vec, const Vector2& highEnd)
		{
			Vector2 newVec = { Mathf::Clamp(vec.x, 0.0f, highEnd.x), Mathf::Clamp(vec.y, 0.0f, highEnd.y) };
			return newVec;
		}
		static inline Vector2 Clamp(const Vector2& vec, const Vector2& lowEnd, const Vector2& highEnd)
		{
			Vector2 newVec = { Mathf::Clamp(vec.x, lowEnd.x, highEnd.x), Mathf::Clamp(vec.y, lowEnd.y, highEnd.y) };
			return newVec;
		}
		static inline Vector2 Ceil(const Vector2& vec)
		{
			Vector2 newVec;
			newVec.x = Mathf::Ceil(vec.x);
			newVec.y = Mathf::Ceil(vec.y);
			return newVec;
		}
	};
}

namespace std
{
	template <>
	struct hash<Advres::RSE::Vector2>
	{
		// Implement hasing function for Vector2 so that I can use it in STL containers
		size_t operator()(const Advres::RSE::Vector2& k) const
		{
			return hash<float>()(k.x) ^ hash<float>()(k.y);
		}
	};
	/*template <> struct formatter<Advres::RSE::Vector2> // C++ 20 thing, we don't need this for now.
	{ 
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const Advres::RSE::Vector2& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
		}
	};*/
}

namespace fmt
{
	template<> struct formatter<Advres::RSE::Vector2>
	{
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const Advres::RSE::Vector2& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
		}
	};
}

#endif // VECTOR2_H