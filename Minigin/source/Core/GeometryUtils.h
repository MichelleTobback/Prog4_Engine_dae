#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class GeometryUtils final
	{
	public:
		struct IntersectionResult
		{
			bool succes{};
			glm::vec2 intersectionPoint{};
			glm::vec2 intersectionNormal{};
			float depth{};
		};

		static IntersectionResult AABBQuadIntersection(const glm::vec4& quad0, const glm::vec4& quad1);
		static bool SATPolygonIntersection(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB, IntersectionResult& result);
		static glm::vec2 ProjectVertices(const std::vector<glm::vec2>& verticesA, const glm::vec2& axis);
		static glm::vec2 ProjectCircle(const glm::vec2& center, float radius, const glm::vec2& axis);
		static glm::vec2 GetCenter(const std::vector<glm::vec2>& vertices);
		static bool SATCirclePolygonIntersection(const glm::vec2& circleCenter, float circleRadius, const std::vector<glm::vec2>& polyVerts, IntersectionResult& result);
		static const glm::vec2& FindClosestPointOnPolygon(const std::vector<glm::vec2>& polyVerts, const glm::vec2& pos);

	private:
		GeometryUtils() = default;
	};
}