#pragma once
#include "roadgeometrypoint.h"
#include <vector>

class Road;

class RoadGeometry
{
    std::vector<RoadGeometryPoint> points_;
public:
    RoadGeometry(Road* road);
    void addPoint(QPointF position, size_t index);

    RoadGeometryPoint& getPoint(size_t index) { return points_.at(index); }
    const RoadGeometryPoint& getPoint(size_t index) const { return points_.at(index); }

private:
    QPointF tangent(const QPointF& p1, const QPointF& p2);
};
