#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <sdafx.h>

#include "Vector2.h"

namespace Advres::RSE
{
	class Transform
	{
	public:
		Vector2 position;
		Vector2 scale;
		float rotation;

	public:
		Transform()
		{
			position.Zero();
			scale = { 1, 1 };
			rotation = 0;
		}
		Transform(Vector2 pos, Vector2 scale = { 1, 1 }, float rotation = 0)
		{
			position = pos;
			this->scale = scale;
			this->rotation = rotation;
		}

	public:
		friend std::ostream& operator<<(std::ostream& stream, Transform& trans)
		{
			stream << "[pos: " << trans.position << " scale: " << trans.scale << " rot: " << trans.rotation << "]";
			return stream;
		}
		inline Transform operator+(const int num)
		{
			Transform t;
			t.position += num;
			t.scale += num;
			t.rotation += num;
			return t;
		}
		inline Transform operator+(const Transform& other)
		{
			Transform t;
			t.position = position + other.position;
			t.scale = scale + other.scale;
			t.rotation = rotation + other.rotation;
			return t;
		}
		inline std::string ToString() const
		{
			return std::string();
			//return std::format("pos: {} sale: {} rot: {}", position, scale, std::to_string(rotation));
		}
		template<typename T>
		inline typename std::enable_if<std::is_same<T, int>::value ||
			std::is_same<T, float>::value, std::string>::type
			ToString() const
		{
			return std::format("pos: {} sale: {} rot: {}", position, scale, std::to_string((int) rotation));
		}
	};
}

namespace std
{
	/*template <> struct formatter<Advres::RSE::Transform> // C++ 20 thing, we don't need this for now.
	{
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const Advres::RSE::Transform& trans, FormatContext& ctx)
		{
			return format_to(ctx.out(), "[pos: {}, scale: {}, rot: {}]", trans.position, trans.scale, trans.rotation);
		}
	};*/
}

namespace fmt
{
	template <> struct formatter<Advres::RSE::Transform>
	{
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const Advres::RSE::Transform& trans, FormatContext& ctx)
		{
			return format_to(ctx.out(), "[pos: {}, scale: {}, rot: {}]", trans.position, trans.scale, trans.rotation);
		}
	};
}

#endif // TRANSFORM_H