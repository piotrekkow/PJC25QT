#include "roadgeometry.h"
#include "road.h"
#include "geometrymanager.h"
#include "intersection.h"
#include <stdexcept>

#include "geometryutils.h"

/**
 * Basic constructor for road geometry, assumes straight connection between intersections, end point intersection offset of 10m
*/
RoadGeometry::RoadGeometry(const Road* road, GeometryManager* geometryManager, qreal startOffset, qreal endOffset)
    : road_{ road }
    , geometryManager_{ geometryManager }
{
    QPointF startPos = road->startIntersection()->position();
    QPointF endPos = road->endIntersection()->position();


    // QLineF line(startPos, endPos);
    // QLineF backwardLine(endPos, startPos);

    // const QPointF unitVectorStart = line.unitVector().p2();
    // const QPointF unitVectorEnd = backwardLine.unitVector().p2();

    QPointF unitVectorStart = tangent(startPos, endPos);
    QPointF unitVectorEnd = tangent(endPos, startPos);

    QPointF startPoint = startPos + (unitVectorStart * startOffset);
    QPointF endPoint = endPos + (unitVectorEnd * endOffset);

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
