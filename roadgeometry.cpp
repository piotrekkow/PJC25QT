#include "roadgeometry.h"
#include "road.h"
#include <stdexcept>
#include <ranges>

/**
 * Basic constructor for road geometry, assumes straight connection between intersections, end point intersection offset of 10m
*/
RoadGeometry::RoadGeometry(Road* road)
{
    const float baseIntersectionOffset = 10.0f;
    const QPointF primaryPos = road->startIntersection()->position();
    const QPointF secondaryPos = road->endIntersection()->position();

    const QPointF unitVector = tangent(primaryPos, secondaryPos);

    const QPointF startPoint = primaryPos + (unitVector * baseIntersectionOffset);
    const QPointF endPoint = secondaryPos - (unitVector * baseIntersectionOffset);

    points_.push_back(RoadGeometryPoint(startPoint));
    points_.push_back(RoadGeometryPoint(endPoint));
}

/**
 * @brief RoadGeometry::laneGeometry
 * @param lane
 * @return polyline of points
 */
QPointF RoadGeometry::calculateLanePoint(const Lane* lane, size_t pointIndex) const
{
    // This function assumes 'lane' is valid and 'pointIndex' is in bounds,
    // as the public-facing methods will perform those checks.
    const Roadway* roadway = lane->roadway();

    const bool isForward = roadway->isForwardRoadway();
    const float offsetSign = isForward ? -1.0f : 1.0f;

    const auto& current = points_[pointIndex];
    const QPointF prevPos = points_[pointIndex == 0 ? pointIndex : pointIndex - 1].position();
    const QPointF nextPos = points_[pointIndex == points_.size() - 1 ? pointIndex : pointIndex + 1].position();

    QPointF tangentVec = nextPos - prevPos;
    float len = std::hypot(tangentVec.x(), tangentVec.y());
    if (len > 1e-6) {
        tangentVec /= len;
    }

    const float offsetMagnitude = current.medianWidth() / 2.0f + current.medianOffset() + lane->cumulativeOffset();
    const QPointF normal(-tangentVec.y(), tangentVec.x());

    return current.position() + normal * offsetMagnitude * offsetSign;
}


// --- REFACTORED PUBLIC FUNCTIONS ---

QPolygonF RoadGeometry::laneGeometry(const Lane* lane) const
{
    if (!lane || !lane->roadway())
        throw std::runtime_error("Provided lane or its parent roadway is invalid.");

    if (points_.size() < 2)
        throw std::runtime_error("Defined number of road points insufficient to produce geometry.");

    QPolygonF lanePoly;
    lanePoly.reserve(points_.size());

    if (lane->roadway()->isForwardRoadway()) {
        for (auto i : std::views::iota(0u, points_.size())) {
            lanePoly << calculateLanePoint(lane, i);
        }
    } else {
        for (auto i : std::views::iota(0u, points_.size()) | std::views::reverse) {
            lanePoly << calculateLanePoint(lane, i);
        }
    }
    return lanePoly;
}

QPointF RoadGeometry::laneStartPoint(const Lane* lane) const
{
    if (!lane || !lane->roadway())
        throw std::runtime_error("Provided lane or its parent roadway is invalid.");

    if (points_.empty())
        throw std::runtime_error("No road points available to calculate start point.");

    // The start point of a forward lane is at index 0.
    // The start point of a backward lane is at the last index.
    const bool isForward = lane->roadway()->isForwardRoadway();
    const size_t startIndex = isForward ? 0 : points_.size() - 1;

    return calculateLanePoint(lane, startIndex);
}

QPointF RoadGeometry::laneEndPoint(const Lane* lane) const
{
    if (!lane || !lane->roadway())
        throw std::runtime_error("Provided lane or its parent roadway is invalid.");

    if (points_.empty())
        throw std::runtime_error("No road points available to calculate end point.");

    // The end point of a forward lane is at the last index.
    // The end point of a backward lane is at index 0.
    const bool isForward = lane->roadway()->isForwardRoadway();
    const size_t endIndex = isForward ? points_.size() - 1 : 0;

    return calculateLanePoint(lane, endIndex);
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
