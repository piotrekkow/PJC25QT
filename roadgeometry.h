#pragma once
#include "roadgeometrypoint.h"
#include <vector>
#include <QPolygonF>

class Road;
class Lane;

class RoadGeometry
{
    std::vector<RoadGeometryPoint> points_;
public:
    RoadGeometry(Road* road);
    void addPoint(QPointF position, size_t index);

    RoadGeometryPoint& pointAt(size_t index) { return points_.at(index); }
    const RoadGeometryPoint& pointAt(size_t index) const { return points_.at(index); }
    const std::vector<RoadGeometryPoint>& points() const { return points_; }
};
