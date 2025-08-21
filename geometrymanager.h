#pragma once

#include <unordered_map>
#include <vector>

#include <QPolygonF>
#include <QPainterPath>
#include <QLineF>

#include "roadwaygeometrycalculator.h" // For OrientedPoint struct

// Forward declarations of the topology classes
class RoadNetwork;
class Road;
class Roadway;
class Lane;
class Connection;

/**
 * @brief A centralized manager for calculating, caching, and querying road network geometry.
 *
 * This class decouples the topological representation of the road network (Road, Lane, etc.)
 * from its geometric representation. It handles on-demand calculation and caching of geometry
 * to improve performance and centralizes all geometry-related logic.
 */
class GeometryManager
{
    RoadNetwork* network_;
    mutable std::unordered_map<const Roadway*, std::vector<OrientedPoint>> roadwayBaselineCache_;
    mutable std::unordered_map<const Roadway*, QPainterPath> roadwayGeometryCache_;
    mutable std::unordered_map<const Lane*, QPainterPath> laneGeometryCache_;
    mutable std::unordered_map<const Connection*, QPainterPath> connectionGeometryCache_;

public:
    explicit GeometryManager(RoadNetwork* network);

    // --- Public Getters ---
    // These methods return a const reference to the cached geometry.
    // If the geometry is not in the cache, they calculate it, store it, and then return it.

    const std::vector<OrientedPoint>& roadwayBaseline(const Roadway* roadway) const;
    const QPainterPath& roadway(const Roadway* roadway) const;
    const QPainterPath& lane(const Lane* lane) const;
    const QPainterPath& connection(const Connection* connection) const;

    /**
     * @brief Invalidates all cached geometry associated with a specific road.
     * This should be called whenever the underlying RoadGeometry of a road changes.
     * @param road The road whose geometry has been modified.
     */
    void invalidate(const Road* road);

    /**
     * @brief Clears all cached geometry.
     */
    void clearAll();

private:
    const std::vector<OrientedPoint>& calculateAndCacheRoadwayBaseline(const Roadway* roadway) const;
    const QPainterPath& calculateAndCacheRoadway(const Roadway* roadway) const;
    const QPainterPath& calculateAndCacheLaneGeometry(const Lane* lane) const;
    const QPainterPath& calculateAndCacheConnectionGeometry(const Connection* connection) const;
};
