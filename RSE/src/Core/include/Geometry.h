#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL.h>

namespace Advres::RSE
{
	// Wrapper around SDL_Rect, why you might ask? Well, why not?
	struct Rect
	{
		int x, y, w, h;

	public:
		Rect(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), w(w), h(h)
		{

		}
		Rect(float x, float y, int w, int h) : x((int)x), y((int)y), w((int)w), h((int)h)
		{

		}
		Rect(SDL_Rect rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
		{

		}

	public:
		inline SDL_Rect SDL() const
		{
			return { x, y, w, h };
		}

	public:
		static inline Rect ToRect(SDL_Rect rect)
		{
			return { rect.x, rect.y, rect.w, rect.h };
		}

	public:
		friend std::ostream& operator<<(std::ostream& stream, const Rect& rect)
		{
			stream << "(" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ")";
			return stream;
		}
		inline bool operator==(const Rect& other) const
		{
			return x == other.x && y == other.y && w == other.w && h == other.h;
		}
	};

	// lol
	struct Point
	{
		int x, y;

	public:
		Point(int x, int y) : x(x), y(y)
		{

		}

	public:
		inline SDL_Point SDL()
		{
			return { x, y };
		}
	};

	struct RGBA
	{
		uint8_t r, g, b, a;
	
	public:
		RGBA(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a)
		{

		}
	};
}

namespace std
{
	template<>
	struct hash<Advres::RSE::Rect>
	{
		// Implement hasing function for Rect so that I can use it in STL containers
		size_t operator()(const Advres::RSE::Rect& r) const
		{
			return hash<int>()(r.x) ^ hash<int>()(r.y) ^ hash<int>()(r.w) ^ hash<int>()(r.h);
		}
	};
}

#endif // GEOMETRY_H