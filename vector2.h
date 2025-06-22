#pragma once

struct Vector2
{
    float x;
    float y;

    Vector2();
    Vector2(float xValue, float yValue);
};

inline Vector2 operator+(const Vector2& v1, const Vector2& v2);
inline Vector2 operator-(const Vector2& v1, const Vector2& v2);
inline Vector2 operator*(const Vector2& v, float scalar);
inline Vector2 operator*(float scalar, const Vector2& v);
inline Vector2 operator/(const Vector2& v, float scalar);
inline Vector2& operator+=(Vector2& v1, const Vector2& v2);
inline Vector2& operator-=(Vector2& v1, const Vector2& v2);
inline Vector2 operator-(const Vector2& v);
inline bool operator==(const Vector2& v1, const Vector2& v2);
inline bool operator!=(const Vector2& v1, const Vector2& v2);
float vector2Length(Vector2 v);
float vector2Distance(Vector2 v1, Vector2 v2);
Vector2 vector2Normalize(Vector2 v);
Vector2 normalizedTangent(Vector2 v1, Vector2 v2);
Vector2 tangent2Normal(Vector2 tangent);
float dot(Vector2 v1, Vector2 v2);
float det(Vector2 v1, Vector2 v2);
float vector2Angle(Vector2 v1, Vector2 v2);
Vector2 quadBezierPoint(Vector2 start, Vector2 control, Vector2 end, float t);
bool lineIntersection(Vector2 p1, Vector2 dir1, Vector2 p2, Vector2 dir2, Vector2& intersectionPoint);
/**
* @brief Calculates the intersection of two line segments.
* @return -1.0f if no intersection, otherwise the distance from p1 to the intersection point.
*/
float lineIntersectionCap(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2);
