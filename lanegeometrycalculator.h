#pragma once

#include <QPainterPath>
#include "roadwaygeometrycalculator.h"

// Forward declarations to avoid including full headers
class Lane;
class RoadGeometry;

class LaneGeometryCalculator
{
public:
    // This class should not be instantiated
    LaneGeometryCalculator() = delete;

    /// less expensive version that uses already calculated baseline roadwaygeometry
    static QPainterPath calculateGeometry(const std::vector<OrientedPoint>& baselineRoadwayGeometry, const Lane* lane, const Roadway* roadway);

    /// more expensive version that calculates baseline roadwaygeometry each call
    static QPainterPath calculateGeometry(const RoadGeometry& roadGeometry, const Lane* lane, const Roadway* roadway);

    static QPointF calculateStartPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway);
    static QPointF calculateEndPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway);

private:
    // The core private helper function, also static
    static QPointF calculatePointForLane(const std::vector<OrientedPoint>& geometry, const Lane* lane, const Roadway* roadway, size_t pointIndex);
    static qreal calculateCumulativeOffset(const Lane* lane, const Roadway* roadway);
};
