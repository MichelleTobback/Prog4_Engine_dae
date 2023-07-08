#include "GeometryUtils.h"
#include "Core/Math.h"

bool dae::GeometryUtils::AABBQuadOverlap(const glm::vec4& quad0, const glm::vec4& quad1)
{
    bool overlapX = (quad0.x < quad1.x + quad1.z) && (quad0.x + quad0.z > quad1.x);
    bool overlapY = (quad0.y < quad1.y + quad1.w) && (quad0.y + quad0.w > quad1.y);

    return (overlapX && overlapY);
}

bool dae::GeometryUtils::SATPolygonIntersection(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB, IntersectionResult& result)
{
    result.depth = FLT_MAX;
    result.intersectionNormal = {};
    for (size_t i{}; i < verticesA.size(); ++i)
    {
        const glm::vec2& v0{ verticesA[i] };
        const glm::vec2& v1{ verticesA[(i + 1) % verticesA.size()]};
        const glm::vec2 edge{ v1 - v0 };
        const glm::vec2 axis{ glm::normalize(glm::vec2{-edge.y, edge.x }) };

        const glm::vec2 projectedA{ ProjectVertices(verticesA, axis) };
        const glm::vec2 projectedB{ ProjectVertices(verticesB, axis) };

        if (projectedA.x >= projectedB.y || projectedB.x >= projectedA.y)
            return false;

        const float depth{std::min(projectedB.y - projectedA.x, projectedA.y - projectedB.x)};
        if (depth < result.depth)
        {
            result.depth = depth;
            result.intersectionNormal = axis;
        }
    }

    for (size_t i{}; i < verticesB.size(); ++i)
    {
        const glm::vec2& v0{ verticesB[i] };
        const glm::vec2& v1{ verticesB[(i + 1) % verticesB.size()] };
        const glm::vec2 edge{ v1 - v0 };
        const glm::vec2 axis{ glm::normalize(glm::vec2{-edge.y, edge.x }) };

        const glm::vec2 projectedA{ ProjectVertices(verticesA, axis) };
        const glm::vec2 projectedB{ ProjectVertices(verticesB, axis) };

        if (projectedA.x >= projectedB.y || projectedB.x >= projectedA.y)
            return false;

        const float depth{ std::min(projectedB.y - projectedA.x, projectedA.y - projectedB.x) };
        if (depth < result.depth)
        {
            result.depth = depth;
            result.intersectionNormal = axis;
        }
    }

    const glm::vec2 centerA{ GetCenter(verticesA) };
    const glm::vec2 centerB{ GetCenter(verticesB) };
    const glm::vec2 direction{ centerB - centerA };

    if (glm::dot(direction, result.intersectionNormal) < 0.f)
        result.intersectionNormal = -result.intersectionNormal;

    return true;
}

glm::vec2 dae::GeometryUtils::ProjectVertices(const std::vector<glm::vec2>& vertices, const glm::vec2& axis)
{
    glm::vec2 result{FLT_MAX, FLT_MIN};

    for (const glm::vec2& vert : vertices)
    {
        float projection{ glm::dot(vert, axis) };

        result.x = std::min(result.x, projection);
        result.y = std::max(result.y, projection);
    }

    return result;
}

glm::vec2 dae::GeometryUtils::ProjectCircle(const glm::vec2& center, float radius, const glm::vec2& axis)
{
    glm::vec2 direction{glm::normalize(axis) * radius };
    glm::vec2 p0{ center + direction };
    glm::vec2 p1{ center - direction };

    glm::vec2 result
    {
        glm::dot(p0, axis),
        glm::dot(p1, axis)
    };
    if (result.x > result.y)
        std::swap(result.x, result.y);

    return result;
}

glm::vec2 dae::GeometryUtils::GetCenter(const std::vector<glm::vec2>& vertices)
{
    glm::vec2 result{};
    for (const glm::vec2& vert : vertices)
    {
        result += vert;
    }
    result /= vertices.size();
    return result;
}

bool dae::GeometryUtils::SATCirclePolygonIntersection(const glm::vec2& circleCenter, float circleRadius, const std::vector<glm::vec2>& polyVerts, IntersectionResult& result)
{
    result.depth = FLT_MAX;
    result.intersectionNormal = {};
    glm::vec2 axis{};

    for (size_t i{}; i < polyVerts.size(); ++i)
    {
        const glm::vec2& v0{ polyVerts[i] };
        const glm::vec2& v1{ polyVerts[(i + 1) % polyVerts.size()] };
        const glm::vec2 edge{ v1 - v0 };
        axis = glm::normalize(glm::vec2{-edge.y, edge.x });

        const glm::vec2 projectedPoly{ ProjectVertices(polyVerts, axis) };
        const glm::vec2 projectedCircle{ ProjectCircle(circleCenter, circleRadius, axis) };

        if (projectedPoly.x >= projectedCircle.y || projectedCircle.x >= projectedPoly.y)
            return false;

        const float depth0{ std::min(projectedCircle.y - projectedPoly.x, projectedPoly.y - projectedCircle.x) };
        if (depth0 < result.depth)
        {
            result.depth = depth0;
            result.intersectionNormal = axis;
        }
    }

    const glm::vec2& closestVert{ FindClosestPointOnPolygon(polyVerts, circleCenter) };
    axis = closestVert - circleCenter;

    const glm::vec2 projectedPoly{ ProjectVertices(polyVerts, axis) };
    const glm::vec2 projectedCircle{ ProjectCircle(circleCenter, circleRadius, axis) };

    if (projectedPoly.x >= projectedCircle.y || projectedCircle.x >= projectedPoly.y)
        return false;

    const float depth1{ std::min(projectedCircle.y - projectedPoly.x, projectedPoly.y - projectedCircle.x) };
    if (depth1 < result.depth)
    {
        result.depth = depth1;
        result.intersectionNormal = axis;
    }

    const glm::vec2 centerPoly{ GetCenter(polyVerts) };
    const glm::vec2 direction{ centerPoly - circleCenter };

    if (glm::dot(direction, result.intersectionNormal) < 0.f)
        result.intersectionNormal = -result.intersectionNormal;

    return true;
}

const glm::vec2& dae::GeometryUtils::FindClosestPointOnPolygon(const std::vector<glm::vec2>& polyVerts, const glm::vec2& pos)
{
    assert(polyVerts.size() > 0 && "Cant find closest vertex if there are no vertices");

    size_t index{ 0 };
    float minDistance{ FLT_MAX };

    for (size_t i{}; i < polyVerts.size(); ++i)
    {
        float distance{ glm::length(pos - polyVerts[i])};
        if (distance < minDistance)
        {
            minDistance = distance;
            index = i;
        }
    }
    return polyVerts[index];
}

bool dae::GeometryUtils::RayCircleIntersection(const Ray& ray, const glm::vec2& circleCenter, float circleRadius, IntersectionResult& result)
{
    glm::vec2 rayOrigin{ ray.origin.x, ray.origin.y };
    glm::vec2 rayDirection{ ray.direction.x, ray.direction.y };

    glm::vec2 circleToRay{ rayOrigin - circleCenter };
    float a{ glm::dot(rayDirection, rayDirection) };
    float b{ 2.0f * glm::dot(rayDirection, circleToRay) };
    float c{ glm::dot(circleToRay, circleToRay) - (circleRadius * circleRadius) };
    float discriminant{ (b * b) - (4.0f * a * c) };

    if (discriminant < 0.0f)
    {
        result.succes = false;
        return false;
    }

    float sqrtDiscriminant{ glm::sqrt(discriminant) };
    float t1{ (-b - sqrtDiscriminant) / (2.0f * a) };
    float t2{ (-b + sqrtDiscriminant) / (2.0f * a) };

    if (t1 >= 0.0f && t1 <= ray.length)
    {
        result.intersectionPoint = rayOrigin + (rayDirection * t1);
        result.intersectionNormal = glm::normalize(result.intersectionPoint - circleCenter);
        result.depth = t1;
        result.succes = true;

        return true;
    }

    if (t2 >= 0.0f && t2 <= ray.length)
    {
        result.intersectionPoint = rayOrigin + (rayDirection * t2);
        result.intersectionNormal = glm::normalize(result.intersectionPoint - circleCenter);
        result.depth = t2;
        result.succes = true;

        return true;
    }

    result.succes = false;
    return false;
}

bool dae::GeometryUtils::RayPolygonIntersection(const Ray& ray, const std::vector<glm::vec2>& polyVerts, IntersectionResult& result)
{
    const float epsilon{ 0.001f };
    size_t nrVertices = static_cast<int>(polyVerts.size());
    if (nrVertices == 0)
    {
        result.succes = false;
        return false;
    }

    std::vector<IntersectionResult> intersections;

    glm::vec4 r1{}, r2{};
    r1.x = std::min(ray.origin.x, ray.origin.x + ray.direction.x * ray.length);
    r1.y = std::min(ray.origin.y, ray.origin.y + ray.direction.y * ray.length);
    r1.z = std::max(ray.origin.x, ray.origin.x + ray.direction.x * ray.length) - r1.x;
    r1.w = std::max(ray.origin.y, ray.origin.y + ray.direction.y * ray.length) - r1.y;

    for (size_t idx = 0; idx <= nrVertices; ++idx)
    {
        glm::vec2 q1 = polyVerts[(idx + 0) % nrVertices];
        glm::vec2 q2 = polyVerts[(idx + 1) % nrVertices];

        r2.x = std::min(q1.x, q2.x);
        r2.y = std::min(q1.y, q2.y);
        r2.z = std::max(q1.x, q2.x) - r2.x;
        r2.w = std::max(q1.y, q2.y) - r2.y;

        if (AABBQuadOverlap(r1, r2))
        {
            float lambda1{};
            float lambda2{};
            if (IntersectLineSegments(glm::vec2(ray.origin), glm::vec2(ray.origin + ray.direction * ray.length), q1, q2, lambda1, lambda2, epsilon))
            {
                if (lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1)
                {
                    IntersectionResult intersection;
                    intersection.intersectionPoint = glm::vec3(ray.origin.x + (ray.direction.x * ray.length * lambda1), ray.origin.y + (ray.direction.y * ray.length * lambda1), 0.0f);
                    intersection.intersectionNormal = glm::normalize(glm::vec2(q2 - q1));
                    intersection.depth = lambda1;
                    intersections.push_back(intersection);
                }
            }
        }
    }

    if (intersections.empty())
    {
        result.succes = false;
        return false;
    }

    result = *std::min_element(intersections.begin(), intersections.end(), [](const IntersectionResult& first, const IntersectionResult& last) 
        {
            return first.depth < last.depth;
        });

    result.succes = true;
    return true;
}

bool dae::GeometryUtils::IntersectLineSegments(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, float& outLambda1, float& outLambda2, float epsilon)
{
    bool intersecting{ false };

    glm::vec2 p1p2{ p2 - p1 };
    glm::vec2 q1q2{ q2 - q1 };

    // Cross product to determine if parallel
    float denom = Math::Cross(p1p2, q1q2);

    // Don't divide by zero
    if (std::abs(denom) > epsilon)
    {
        intersecting = true;

        glm::vec2 p1q1{ q1 - p1 };

        float num1 = Math::Cross(p1q1, q1q2);
        float num2 = Math::Cross(p1q1, p1p2);
        outLambda1 = num1 / denom;
        outLambda2 = num2 / denom;
    }
    else // are parallel
    {
        // Connect start points
        glm::vec2 p1q1{ q1 - p1 };

        // Cross product to determine if segments and the line connecting their start points are parallel, 
        // if so, than they are on a line
        // if not, then there is no intersection
        if (std::abs(Math::Cross(p1q1, q1q2) > epsilon))
        {
            return false;
        }

        // Check the 4 conditions
        outLambda1 = 0;
        outLambda2 = 0;
        if (IsPointOnLineSegment(p1, q1, q2) ||
            IsPointOnLineSegment(p2, q1, q2) ||
            IsPointOnLineSegment(q1, p1, p2) ||
            IsPointOnLineSegment(q2, p1, p2))
        {
            intersecting = true;
        }
    }
    return intersecting;
}

bool dae::GeometryUtils::IsPointOnLineSegment(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b)
{
    glm::vec2 ap{ p - a }, bp{ p - b };
    // If not on same line, return false
    if (abs(Math::Cross(ap, bp)) > 0.001f)
    {
        return false;
    }

    // Both vectors must point in opposite directions if p is between a and b
    if (glm::dot(ap, bp) > 0)
    {
        return false;
    }

    return true;
}
