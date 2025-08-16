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
