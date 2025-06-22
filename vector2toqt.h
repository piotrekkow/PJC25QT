#pragma once
#include <QPointF>
#include "vector2.h"

inline QPointF toQPointF(const Vector2& v)
{
    return QPointF(v.x, v.y);
}

inline Vector2 toVector2(const QPointF& p)
{
    return Vector2(static_cast<float>(p.x()), static_cast<float>(p.y()));
}
