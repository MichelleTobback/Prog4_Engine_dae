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

		inline static bool isPointInsideRect(const glm::vec2& point, const glm::vec2& rectPosition, const glm::vec2& rectSize) 
		{
			float left = rectPosition.x;
			float right = rectPosition.x + rectSize.x;
			float top = rectPosition.y;
			float bottom = rectPosition.y + rectSize.y;

			return (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom);
		}

		inline static float WrapAngle(float angle)
		{
			angle = std::fmodf(angle, 360.0f);
			if (angle < 0.0f)
			{
				angle += 360.0f;
			}
			return angle;
		}

	private:
		Math() = default;
	};
}