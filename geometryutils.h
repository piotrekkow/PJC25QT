#pragma once
#include <QPointF>
#include <cmath>

QPointF tangent(const QPointF& p1, const QPointF& p2)
{
    QPointF tangent = p2 - p1;
    double len = std::hypot(tangent.x(), tangent.y());
    if (len > 1e-6) {
        tangent /= len;
        return tangent;
    }
    return QPointF(0.0, 0.0);
}

QPointF normal(const QPointF& p1, const QPointF& p2)
{
    QPointF tangentVec = tangent(p1, p2);
    return QPointF(-tangentVec.y(), tangentVec.x());
}
