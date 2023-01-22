#ifndef MATHF_H
#define MATHF_H

#include <cmath>

namespace RSE
{
	class Mathf
	{
	public:
		template<typename T>
		static float Abs(T x) noexcept
		{
			return (float) std::abs(x);
		}
		template<typename T>
		static float Ceil(T x) noexcept
		{
			return (float) std::ceil(x);
		}
		template<typename T>
		static float Floor(T x) noexcept
		{
			return (float) std::floor(x);
		}
		template<typename T>
		static float Pow(T x, T y) noexcept
		{
			return (float) std::pow(x, y);
		}
		template<typename T>
		static float Sqrt(T x) noexcept
		{
			return (float) std::sqrt(x);
		}
		template<typename T>
		static float Clamp(const T x, const T lowEnd, const T highEnd) noexcept
		{
			if (x < lowEnd) return lowEnd;
			else if (x > highEnd) return highEnd;
			else return x;
		}
		template<typename T>
		static float Round(const T x)
		{
			return (float) std::round(x);
		}
		template<typename T>
		static float Max(const T x, const T y)
		{
			return (float) (((x) < (y)) ? (x) : (y));
		}
		template<typename T>
		static float Min(const T x, const T y)
		{
			return (float) (((x) < (y)) ? (x) : (y));
		}
	};
}

#endif // MATH_F