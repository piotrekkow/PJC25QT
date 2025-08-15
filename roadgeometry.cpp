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
QPolygonF RoadGeometry::laneGeometry(const Lane* lane) const
    {
        const Roadway* roadway = lane->roadway();
        if (!roadway)
            std::runtime_error("Provided lane has no parent roadway.");

        if (points_.size() < 2)
            std::runtime_error("Defined number of road points insufficient to produce geometry.");

        bool isForward = roadway->isForwardRoadway();
        const float offsetSign = isForward ? -1.0f : 1.0f;

        QPolygonF laneGeometry;
        laneGeometry.reserve(points_.size());

        // Define the work to be done for each point inside a lambda
        // This avoids duplicating the logic inside an if/else block
        auto processPoint = [&](size_t i) {
            const auto& current = points_[i];
            QPointF prevPos = points_[i == 0 ? i : i - 1].position();
            QPointF nextPos = points_[i == points_.size() - 1 ? i : i + 1].position();

            QPointF tangent = nextPos - prevPos;
            float len = std::hypot(tangent.x(), tangent.y());
            if (len > 1e-6) {
                tangent /= len;
            }

            float offsetMagnitude = current.medianWidth() / 2.0f + current.medianOffset() + lane->cumulativeOffset();
            QPointF normal(-tangent.y(), tangent.x());
            laneGeometry << current.position() + normal * offsetMagnitude * offsetSign;
        };

        if (isForward) {
            for (auto i : std::views::iota(0u, points_.size())) {
                processPoint(i);
            }
        } else {
            for (auto i : std::views::iota(0u, points_.size()) | std::views::reverse) {
                processPoint(i);
            }
        }

        return laneGeometry;
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
