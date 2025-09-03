#pragma once
#include "roadgeometrypoint.h"
#include <vector>

class Road;
class Lane;
class GeometryManager;

class RoadGeometry
{
    std::vector<RoadGeometryPoint> points_;
    const Road* road_;
    GeometryManager* geometryManager_;

public:
    RoadGeometry(const Road* road, GeometryManager* geometryManager, qreal startOffset = 15.0, qreal endOffset = 15.0);
    void addPoint(QPointF position, size_t index);

    RoadGeometryPoint& pointAt(size_t index) { return points_.at(index); }
    const RoadGeometryPoint& pointAt(size_t index) const { return points_.at(index); }
    const std::vector<RoadGeometryPoint>& points() const { return points_; }
};
