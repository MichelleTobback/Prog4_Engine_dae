#pragma once
#include <glm/glm.hpp>
#include <math.h>

namespace dae
{
	class Math final
	{
	public:
		template <typename T>
		inline static T Lerp(const T& a, const T& b, float t)
		{
			return a + (b - a) * t;
		}

		inline static bool AreSignsEqual(float x, float y)
		{
			return std::signbit(x) == std::signbit(y);
		}
	};
}