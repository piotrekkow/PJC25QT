#include "roadgeometry.h"
#include "road.h"
#include "geometryutils.h"

/**
 * Basic constructor for road geometry, assumes straight connection between intersections, end point intersection offset of 10m
*/
RoadGeometry::RoadGeometry(Road* road, float startOffset, float endOffset)
{
    const QPointF startPos = road->startIntersection()->position();
    const QPointF endPos = road->endIntersection()->position();

    const QPointF unitVector = tangent(startPos, endPos);

    const QPointF startPoint = startPos + (unitVector * startOffset);
    const QPointF endPoint = endPos - (unitVector * endOffset);

    points_.push_back(RoadGeometryPoint(startPoint));
    points_.push_back(RoadGeometryPoint(endPoint));
}

void RoadGeometry::addPoint(QPointF position, size_t index)
{
    // The index must be valid for insertion. It can be any position *between*
    // the start and end points, hence `index > 0` and `index < points_.size()`.
    if (index == 0 || index >= points_.size())
    {
        throw std::out_of_range("Cannot insert a point at the start or end of the road geometry, or out of bounds.");
    }

    // Create an iterator pointing to the insertion position.
    auto it = points_.begin() + index;

    // Insert the new RoadGeometryPoint at the specified location.
    // The default median width and offset will be 0.0f.
    points_.insert(it, RoadGeometryPoint(position));
}
