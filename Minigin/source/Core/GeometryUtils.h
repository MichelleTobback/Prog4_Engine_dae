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
			glm::vec2 intersectionPoint{};
			glm::vec2 intersectionNormal{};
			float depth{};
			bool succes{};
		};
		struct Ray
		{
			glm::vec3 origin{};
			glm::vec3 direction{};
			float length{};
		};

		static bool AABBQuadOverlap(const glm::vec4& quad0, const glm::vec4& quad1);
		static bool SATPolygonIntersection(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB, IntersectionResult& result);
		static glm::vec2 ProjectVertices(const std::vector<glm::vec2>& verticesA, const glm::vec2& axis);
		static glm::vec2 ProjectCircle(const glm::vec2& center, float radius, const glm::vec2& axis);
		static glm::vec2 GetCenter(const std::vector<glm::vec2>& vertices);
		static bool SATCirclePolygonIntersection(const glm::vec2& circleCenter, float circleRadius, const std::vector<glm::vec2>& polyVerts, IntersectionResult& result);
		static const glm::vec2& FindClosestPointOnPolygon(const std::vector<glm::vec2>& polyVerts, const glm::vec2& pos);
		static bool RayCircleIntersection(const Ray& ray, const glm::vec2& circleCenter, float circleRadius, IntersectionResult& result);
		static bool RayPolygonIntersection(const Ray& ray, const std::vector<glm::vec2>& polyVerts, IntersectionResult& result);
		static bool IntersectLineSegments(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, float& outLambda1, float& outLambda2, float epsilon);
		static bool IsPointOnLineSegment(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b);
	private:
		GeometryUtils() = default;
	};
}