#include "vector2.h"
#include <cmath>

Vector2::Vector2()
    : x{ 0.0f }
    , y{ 0.0f }
{}

Vector2::Vector2(float xValue, float yValue)
    : x{ xValue }
    , y{ yValue }
{}


inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return { v1.x - v2.x, v1.y - v2.y };
}

inline Vector2 operator*(const Vector2& v, float scalar)
{
    return { v.x * scalar, v.y * scalar };
}

inline Vector2 operator*(float scalar, const Vector2& v)
{
    return { v.x * scalar, v.y * scalar };
}

inline Vector2 operator/(const Vector2& v, float scalar)
{
    if (scalar != 0.0f)
        return { v.x / scalar, v.y / scalar };
    return v;
}

inline Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

inline Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

inline Vector2 operator-(const Vector2& v)
{
    return { -v.x, -v.y };
}

inline bool operator==(const Vector2& v1, const Vector2& v2)
{
    const float precision = 0.00001f; // e-5 precision

    // near zero comparison
    if (std::abs(v1.x) < precision && std::abs(v2.x) < precision &&
        std::abs(v1.y) < precision && std::abs(v2.y) < precision) {
        return true;
    }

    // remaining cases
    float xEpsilon = std::abs(std::max(std::abs(v1.x), std::abs(v2.x)) * precision);
    float yEpsilon = std::abs(std::max(std::abs(v1.y), std::abs(v2.y)) * precision);

    return (std::abs(v1.x - v2.x) < xEpsilon) &&
           (std::abs(v1.y - v2.y) < yEpsilon);
}

inline bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}

float vector2Length(Vector2 v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float vector2Distance(Vector2 v1, Vector2 v2)
{
    return std::sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

Vector2 vector2Normalize(Vector2 v)
{
    float length = vector2Length(v);
    if (length != 0.0f)
        return { v.x / length, v.y / length };
    return v;
}

Vector2 normalizedTangent(Vector2 v1, Vector2 v2)
{
    return vector2Normalize(v2 - v1);
}

Vector2 tangent2Normal(Vector2 tangent)
{
    return { tangent.y, -tangent.x }; // instead of return { -tangent.y, tangent.x }; - consistant with Screen coordinate system where x right, y is down.
}

float dot(Vector2 v1, Vector2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float det(Vector2 v1, Vector2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

float vector2Angle(Vector2 v1, Vector2 v2)
{
    if (vector2Length(v1) < 1e-3f || vector2Length(v2) < 1e-3f) return 0.0f;
    float cosTheta{ dot(v1, v2) / (std::sqrt(vector2Length(v1)) * std::sqrt(vector2Length(v2))) };
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    return std::acos(cosTheta);
}

Vector2 quadBezierPoint(Vector2 start, Vector2 control, Vector2 end, float t)
{
    t = std::max(0.0f, std::min(t, 1.0f));
    float u{ 1.0f - t };
    float tt{ t * t };
    float uu{ u * u };

    return {
        uu * start.x + 2 * t * u * control.x + tt * end.x,
        uu * start.y + 2 * t * u * control.y + tt * end.y
    };
}

bool lineIntersection(Vector2 p1, Vector2 dir1, Vector2 p2, Vector2 dir2, Vector2& intersectionPoint)
{
    if (std::abs(det(dir1, dir2)) < 1e-3f) return false; // lines parallel

    Vector2 diff{ p2 - p1 };
    float t{ (diff.x * dir2.y - diff.y * dir2.x) / det(dir1, dir2) };
    intersectionPoint = p1 + dir1 * t;
    return true;
}

/**
* @brief Calculates the intersection of two line segments.
* @return -1.0f if no intersection, otherwise the distance from p1 to the intersection point.
*/
float lineIntersectionCap(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2)
{
    const float epsilon = 1e-5f;

    // direction vectors
    Vector2 r{ p2 - p1 };
    Vector2 s{ q2 - q1 };

    float r_cross_s = det(r, s);

    if (std::abs(r_cross_s) < epsilon)
    {
        // Lines are parallel or collinear.
        return -1.0f;
    }

    Vector2 c = q1 - p1;

    // Parametric distance p1 to intersection along r
    float t = det(c, s) / r_cross_s;
    // Parametric distance q1 to intersection along s
    float u = det(c, r) / r_cross_s;

    // Check if intersection is within both line segments using epsilon tolerance
    // Allows intersections slightly outside the strict [0, 1] range due to precision errors
    if (t >= -epsilon && t <= (1.0f + epsilon) &&
        u >= -epsilon && u <= (1.0f + epsilon))
    {
        return t;
    }

    return -1.0f; // no intersection within segment bounds
}
