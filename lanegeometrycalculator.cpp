#include "lanegeometrycalculator.h"
#include "roadway.h"
#include "lane.h"
#include "roadwayutils.h"

QPolygonF LaneGeometryCalculator::calculateGeometry(const RoadGeometry& roadGeometry, const Lane* lane, const Roadway* roadway)
{
    std::vector<OrientedPoint> roadwayBaseline = RoadwayGeometryCalculator::calculateBaseline(roadGeometry, roadway);
    return calculateGeometry(roadwayBaseline, lane, roadway);
}

QPolygonF LaneGeometryCalculator::calculateGeometry(const std::vector<OrientedPoint>& roadwayBaseline, const Lane* lane, const Roadway* roadway)
{
    const float laneOffset = calculateCumulativeOffset(lane, roadway);

    QPolygonF lanePolyline;
    lanePolyline.reserve(roadwayBaseline.size());

    for (const auto& baselinePoint : roadwayBaseline)
    {
        lanePolyline << baselinePoint.position + baselinePoint.normal * laneOffset;
    }
    return lanePolyline;
}

QPointF LaneGeometryCalculator::calculatePointForLane(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway, size_t pointIndex)
{
    const float laneOffset = calculateCumulativeOffset(lane, roadway);
    return { geometry[pointIndex].position + geometry[pointIndex].normal * laneOffset };
}

float LaneGeometryCalculator::calculateCumulativeOffset(const Lane* lane, const Roadway* roadway)
{
    float offset = 0.0f;
    const size_t laneIndex = RoadwayUtils::getLaneIndex(lane, roadway);
    const auto& allLanes = roadway->lanes();

    for (size_t i = 0; i < laneIndex; ++i)
    {
        offset += allLanes[i]->width();
    }
    offset += lane->width() / 2.0f;
    return offset;
}
