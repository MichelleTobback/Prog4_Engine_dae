#include "GeometryUtils.h"

dae::GeometryUtils::IntersectionResult dae::GeometryUtils::AABBQuadIntersection(const glm::vec4& quad0, const glm::vec4& quad1)
{
    IntersectionResult result{};

    bool overlapX = (quad0.x < quad1.x + quad1.z) && (quad0.x + quad0.z > quad1.x);
    bool overlapY = (quad0.y < quad1.y + quad1.w) && (quad0.y + quad0.w > quad1.y);

    if (overlapX && overlapY) 
    {
        result.succes = true;

        float intersectionLeft = std::max(quad0.x, quad1.x);
        float intersectionRight = std::min(quad0.x + quad0.z, quad1.x + quad1.z);
        float intersectionTop = std::max(quad0.y, quad1.y);
        float intersectionBottom = std::min(quad0.y + quad0.w, quad1.y + quad1.w);

        glm::vec2 intersectionCenter(
            (intersectionLeft + intersectionRight) * 0.5f,
            (intersectionTop + intersectionBottom) * 0.5f
        );

        float diffLeft = intersectionCenter.x - quad0.x;
        float diffRight = quad0.x + quad0.z - intersectionCenter.x;
        float diffTop = intersectionCenter.y - quad0.y;
        float diffBottom = quad0.y + quad0.w - intersectionCenter.y;

        float minDiff = std::min({ diffLeft, diffRight, diffTop, diffBottom });

        if (minDiff == diffLeft)
        {
            result.intersectionPoint = glm::vec2(quad0.x, glm::clamp(intersectionCenter.y, quad0.y, quad0.y + quad0.w));
            result.intersectionNormal = glm::vec2(-1.0f, 0.0f);
        }
        else if (minDiff == diffRight)
        {
            result.intersectionPoint = glm::vec2(quad0.x + quad0.z, glm::clamp(intersectionCenter.y, quad0.y, quad0.y + quad0.w));
            result.intersectionNormal = glm::vec2(1.0f, 0.0f);
        }
        else if (minDiff == diffTop)
        {
            result.intersectionPoint = glm::vec2(glm::clamp(intersectionCenter.x, quad0.x, quad0.x + quad0.z), quad0.y);
            result.intersectionNormal = glm::vec2(0.0f, -1.0f);
        }
        else // diffBottom is the minimum difference
        {
            result.intersectionPoint = glm::vec2(glm::clamp(intersectionCenter.x, quad0.x, quad0.x + quad0.z), quad0.y + quad0.w);
            result.intersectionNormal = glm::vec2(0.0f, 1.0f);
        }
    }
    
    return result;
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
