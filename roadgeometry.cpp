#include "roadgeometry.h"
#include "road.h"

/**
 * Basic constructor for road geometry, assumes straight connection between intersections, end point intersection offset of 5m
*/
RoadGeometry::RoadGeometry(Road* road)
{
    const float baseIntersectionOffset = 5.0f;
    const QPointF primaryPos = road->getPrimaryIntersection()->getPosition();
    const QPointF secondaryPos = road->getSecondaryIntersection()->getPosition();

    const QPointF unitVector = tangent(primaryPos, secondaryPos);

    const QPointF startPoint = primaryPos + (unitVector * baseIntersectionOffset);
    const QPointF endPoint = secondaryPos - (unitVector * baseIntersectionOffset);

    points_.push_back(RoadGeometryPoint(startPoint));
    points_.push_back(RoadGeometryPoint(endPoint));
}

QPointF RoadGeometry::tangent(const QPointF& p1, const QPointF& p2)
{
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double length = std::sqrt(dx * dx + dy * dy);

    if (length == 0.0)
        return QPointF(0.0, 0.0);

    return QPointF(dx / length, dy / length);
}
