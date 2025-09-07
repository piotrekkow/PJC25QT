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

    /**
     * @brief adds point to road geometry
     * @param position of point
     * @param index at which point will be placed, cannot be 0 or greater than current number of points.
     */
    void addPoint(QPointF position, size_t index);

    /**
     * @return data about point at index
     */
    RoadGeometryPoint& pointAt(size_t index) { return points_.at(index); }
    const RoadGeometryPoint& pointAt(size_t index) const { return points_.at(index); }

    /**
     * @return vector of all points with data in road geometry
     */
    const std::vector<RoadGeometryPoint>& points() const { return points_; }
};
