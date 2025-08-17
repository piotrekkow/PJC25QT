#pragma once

#include <unordered_map>
#include <vector>

#include <QPolygonF>
#include <QLineF>

#include "roadwaygeometrycalculator.h" // For OrientedPoint struct

// Forward declarations of the topology classes
class RoadNetwork;
class Road;
class Roadway;
class Lane;
class IntersectionConnection;

/**
 * @brief A centralized manager for calculating, caching, and querying road network geometry.
 *
 * This class decouples the topological representation of the road network (Road, Lane, etc.)
 * from its geometric representation. It handles on-demand calculation and caching of geometry
 * to improve performance and centralizes all geometry-related logic.
 */
class GeometryManager
{
    // A raw pointer is fine here as the manager's lifetime is tied to the RoadNetwork's.
    RoadNetwork* network_;

    // --- Caches ---
    // We use raw pointers as keys. This is safe because the lifetime of the topology objects
    // is managed by the RoadNetwork, which outlives the GeometryManager.

    mutable std::unordered_map<const Roadway*, std::vector<OrientedPoint>> roadwayBaselineCache_;
    mutable std::unordered_map<const Lane*, QPolygonF> laneGeometryCache_;
    mutable std::unordered_map<const IntersectionConnection*, QLineF> connectionGeometryCache_;
public:
    explicit GeometryManager(RoadNetwork* network);

    // --- Public Getters ---
    // These methods return a const reference to the cached geometry.
    // If the geometry is not in the cache, they calculate it, store it, and then return it.

    const std::vector<OrientedPoint>& roadwayBaseline(const Roadway* roadway) const;
    const QPolygonF& lane(const Lane* lane) const;
    const QLineF& connection(const IntersectionConnection* connection) const;

    // --- Cache Management ---

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
    // --- Private Calculation Methods ---
    // These methods perform the actual calculations using the stateless calculators.

    const std::vector<OrientedPoint>& calculateAndCacheRoadwayBaseline(const Roadway* roadway) const;
    const QPolygonF& calculateAndCacheLaneGeometry(const Lane* lane) const;
    const QLineF& calculateAndCacheConnectionGeometry(const IntersectionConnection* connection) const;
};
