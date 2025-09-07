#pragma once

#include <QPainterPath>
#include "roadwaygeometrycalculator.h"

// Forward declarations to avoid including full headers
class Lane;
class RoadGeometry;

/**
 * @brief Stateless calculator for lane geometry. Lane geometry is entirely dependent on the geometry of the road, specifically a roadways baseline geometry.
 */
class LaneGeometryCalculator
{
public:
    // This class should not be instantiated
    LaneGeometryCalculator() = delete;

    /**
     * @brief less expensive version that uses already calculated baseline roadwaygeometry
     * @param baselineRoadwayGeometry
     * @param lane
     * @param roadway
     * @return lane path geometry with elements indicating center of lane
     */
    static QPainterPath calculateGeometry(const std::vector<OrientedPoint>& baselineRoadwayGeometry, const Lane* lane, const Roadway* roadway);

    /**
     * @brief more expensive version that calculates baseline roadwaygeometry each call
     * @param roadGeometry
     * @param lane
     * @param roadway
     * @return lane path geometry with elements indicating center of lane
     */
    static QPainterPath calculateGeometry(const RoadGeometry& roadGeometry, const Lane* lane, const Roadway* roadway);

    static QPointF calculateStartPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway);
    static QPointF calculateEndPoint(const std::vector<OrientedPoint> &geometry, const Lane *lane, const Roadway *roadway);

private:
    // The core private helper function, also static
    static QPointF calculatePointForLane(const std::vector<OrientedPoint>& geometry, const Lane* lane, const Roadway* roadway, size_t pointIndex);
    static qreal calculateCumulativeOffset(const Lane* lane, const Roadway* roadway);
};
