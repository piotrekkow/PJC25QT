#include "geometrymanager.h"

// You'll need to include the full definitions of your topology classes here
#include "roadnetwork.h"
#include "road.h"
#include "roadway.h"
#include "lane.h"
#include "intersectionconnection.h"

// And your stateless calculators
#include "lanegeometrycalculator.h"

GeometryManager::GeometryManager(RoadNetwork* network)
    : network_{ network }
{
}

// --- Public Getters Implementation ---

const std::vector<OrientedPoint>& GeometryManager::roadwayBaseline(const Roadway* roadway) const
{
    // Check if the baseline is already in the cache
    if (auto it = roadwayBaselineCache_.find(roadway); it != roadwayBaselineCache_.end())
    {
        return it->second;
    }
    // If not, calculate, cache, and return it
    return calculateAndCacheRoadwayBaseline(roadway);
}

const QPolygonF& GeometryManager::lane(const Lane* lane) const
{
    if (auto it = laneGeometryCache_.find(lane); it != laneGeometryCache_.end())
    {
        return it->second;
    }
    return calculateAndCacheLaneGeometry(lane);
}

const QLineF& GeometryManager::connection(const IntersectionConnection* connection) const
{
    if (auto it = connectionGeometryCache_.find(connection); it != connectionGeometryCache_.end())
    {
        return it->second;
    }
    return calculateAndCacheConnectionGeometry(connection);
}


// --- Cache Management Implementation ---

void GeometryManager::invalidate(const Road* road)
{
    // Iterate through all roadways of the given road
    for (const auto& roadway : road->roadways())
    {
        // Erase the roadway baseline from the cache
        roadwayBaselineCache_.erase(roadway);

        // Iterate through all lanes of the roadway
        for (const auto& lane : roadway->lanes())
        {
            // Erase the lane geometry from the cache
            laneGeometryCache_.erase(lane.get());

            // Iterate through all connections from that lane
            for (const auto& connection : lane->connections())
            {
                // Erase the connection geometry from the cache
                connectionGeometryCache_.erase(connection.get());
            }
        }
    }
}

void GeometryManager::clearAll()
{
    roadwayBaselineCache_.clear();
    laneGeometryCache_.clear();
    connectionGeometryCache_.clear();
}


// --- Private Calculation Methods Implementation ---

const std::vector<OrientedPoint>& GeometryManager::calculateAndCacheRoadwayBaseline(const Roadway* roadway) const
{
    // Get the parent road's geometry
    const RoadGeometry& roadGeom = roadway->road()->geometry();
    // Calculate using the existing stateless calculator
    auto baseline = RoadwayGeometryCalculator::calculateBaseline(roadGeom, roadway);
    // Insert into the cache and return the newly inserted element
    return roadwayBaselineCache_.emplace(roadway, std::move(baseline)).first->second;
}

const QPolygonF& GeometryManager::calculateAndCacheLaneGeometry(const Lane* lane) const
{
    // First, ensure the parent roadway's baseline is available (it will be calculated and cached if not)
    const auto& roadwayGeometry = roadwayBaseline(lane->roadway());
    // Now, calculate the lane geometry
    auto laneGeom = LaneGeometryCalculator::calculateGeometry(roadwayGeometry, lane, lane->roadway());
    return laneGeometryCache_.emplace(lane, std::move(laneGeom)).first->second;
}

const QLineF& GeometryManager::calculateAndCacheConnectionGeometry(const IntersectionConnection* connection) const
{
    const QPolygonF& sourceLaneGeom = lane(connection->source());
    const QPolygonF& destLaneGeom = lane(connection->destination());

    if (sourceLaneGeom.isEmpty() || destLaneGeom.isEmpty())
        std::runtime_error("Could not get lane geometry for connection calculation.");

    QPointF sourceEndPoint = sourceLaneGeom.back();
    QPointF destStartPoint = destLaneGeom.front();

    QLineF connectionGeom(sourceEndPoint, destStartPoint);
    return connectionGeometryCache_.emplace(connection, std::move(connectionGeom)).first->second;
}
