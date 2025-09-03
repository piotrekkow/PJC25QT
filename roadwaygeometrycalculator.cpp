#include "roadwaygeometrycalculator.h"
#include "roadway.h"
#include "roadgeometry.h"
#include "geometryutils.h"
#include <stdexcept>
#include <ranges>

// --- Public Member Function ---

std::vector<OrientedPoint> RoadwayGeometryCalculator::calculateBaseline(const RoadGeometry& roadGeometry, const Roadway* roadway)
{
    if (!roadway)
    {
        throw std::invalid_argument("RoadwayGeometryCalculator::calculateBaseline(): Provided roadway is null.");
    }

    const auto& points = roadGeometry.points();
    if (points.size() < 2)
    {
        return {};
    }

    std::vector<OrientedPoint> roadwayBaseline;
    roadwayBaseline.reserve(points.size());

    if (roadway->isForwardRoadway())
    {
        for (auto i : std::views::iota(0u, points.size()))
        {
            roadwayBaseline.emplace_back(calculateBaselinePoint(roadGeometry, roadway, i));
        }
    }
    else
    {
        for (auto i : std::views::iota(0u, points.size()) | std::views::reverse)
        {
            roadwayBaseline.emplace_back(calculateBaselinePoint(roadGeometry, roadway, i));
        }
    }
    return roadwayBaseline;
}


// --- Private Helper Function ---

OrientedPoint RoadwayGeometryCalculator::calculateBaselinePoint(const RoadGeometry& roadGeometry, const Roadway* roadway, size_t pointIndex)
{
    const auto& points = roadGeometry.points();
    const auto& currentPoint = points.at(pointIndex);

    // Determines the adjacent points to calculate the tangent and normal.
    // This logic correctly handles the start and end points of the road.
    const QPointF& prevPos = (pointIndex == 0) ? currentPoint.position() : points.at(pointIndex - 1).position();
    const QPointF& nextPos = (pointIndex == points.size() - 1) ? currentPoint.position() : points.at(pointIndex + 1).position();

    // The direction of the offset depends on whether it's a forward or backward roadway.
    const QPointF smoothNormal = roadway->isForwardRoadway() ? normal(prevPos, nextPos) : normal(nextPos, prevPos);
    const qreal offsetMagnitude = currentPoint.medianWidth() / 2.0 + currentPoint.medianOffset();
    const QPointF offsetVector = smoothNormal * offsetMagnitude;

    return {
        currentPoint.position() + offsetVector, // The final calculated point
        smoothNormal                            // The smooth normal from the centerline
    };
}
