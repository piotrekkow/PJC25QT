#include "lanegeometrycalculator.h"
#include "roadway.h"
#include "lane.h"
#include "roadwayutils.h"

QPainterPath LaneGeometryCalculator::calculateGeometry(const RoadGeometry& roadGeometry, const Lane* lane, const Roadway* roadway)
{
    std::vector<OrientedPoint> roadwayBaseline = RoadwayGeometryCalculator::calculateBaseline(roadGeometry, roadway);
    return calculateGeometry(roadwayBaseline, lane, roadway);
}

QPointF LaneGeometryCalculator::calculateStartPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway)
{
    return calculatePointForLane(geometry, lane, roadway, 0);
}

QPointF LaneGeometryCalculator::calculateEndPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway)
{
    return calculatePointForLane(geometry, lane, roadway, geometry.size()-1);
}


QPainterPath LaneGeometryCalculator::calculateGeometry(const std::vector<OrientedPoint>& roadwayBaseline, const Lane* lane, const Roadway* roadway)
{
    if (roadwayBaseline.empty())
    {
        return QPainterPath();
    }

    const qreal laneOffset = calculateCumulativeOffset(lane, roadway);

    QPainterPath lanePath;
    QPointF firstPoint = roadwayBaseline.front().position + roadwayBaseline.front().normal * laneOffset;
    lanePath.moveTo(firstPoint);

    for (size_t i = 1; i < roadwayBaseline.size(); ++i)
    {
        const auto& baselinePoint = roadwayBaseline[i];
        lanePath.lineTo(baselinePoint.position + baselinePoint.normal * laneOffset);
    }

    return lanePath;
}

QPointF LaneGeometryCalculator::calculatePointForLane(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway, size_t pointIndex)
{
    const qreal laneOffset = calculateCumulativeOffset(lane, roadway);
    return { geometry[pointIndex].position + geometry[pointIndex].normal * laneOffset };
}

qreal LaneGeometryCalculator::calculateCumulativeOffset(const Lane* lane, const Roadway* roadway)
{
    qreal offset = 0.0;
    const size_t laneIndex = RoadwayUtils::getLaneIndex(lane, roadway);
    const auto& allLanes = roadway->lanes();

    for (size_t i = 0; i < laneIndex; ++i)
    {
        offset += allLanes[i]->width();
    }
    offset += lane->width() / 2.0;
    return offset;
}
