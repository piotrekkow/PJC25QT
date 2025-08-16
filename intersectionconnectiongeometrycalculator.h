#pragma once
#include "lanegeometrycalculator.h"
#include "intersectionconnection.h"
#include <QLineF>

class IntersectionConnectionGeometryCalculator
{
public:
    IntersectionConnectionGeometryCalculator() = delete;
    static QLineF calculateGeometry(const QPointF& sourceEndPoint, const IntersectionConnection* connection);
};
