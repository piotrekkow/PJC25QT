#include "roadgeometry.h"
#include "road.h"
#include "geometrymanager.h"
#include "intersection.h"
#include <stdexcept>

/**
 * Basic constructor for road geometry, assumes straight connection between intersections, end point intersection offset of 10m
*/
RoadGeometry::RoadGeometry(const Road* road, GeometryManager* geometryManager, qreal startOffset, qreal endOffset)
    : road_{ road }
    , geometryManager_{ geometryManager }
{
    const QPointF startPos = road->startIntersection()->position();
    const QPointF endPos = road->endIntersection()->position();


    QLineF line(startPos, endPos);

    const QPointF unitVector = line.unitVector().p2();
    const QPointF startPoint = startPos + (unitVector * startOffset);
    const QPointF endPoint = endPos - (unitVector * endOffset);

    points_.push_back(RoadGeometryPoint(startPoint));
    points_.push_back(RoadGeometryPoint(endPoint));
}

void RoadGeometry::addPoint(QPointF position, size_t index)
{
    if (index == 0 || index >= points_.size())
    {
        throw std::out_of_range("Cannot insert a point at the start or end of the road geometry, or out of bounds.");
    }

    auto it = points_.begin() + index;
    points_.insert(it, RoadGeometryPoint(position));

    if (geometryManager_)
    {
        geometryManager_->invalidate(road_);
    }
    else
    {
        throw std::invalid_argument("Invalid pointer to geometry manager, cannot update geometry");
    }
}
