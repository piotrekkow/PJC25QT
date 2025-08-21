#include "geometrymanager.h"

// You'll need to include the full definitions of your topology classes here
#include "roadnetwork.h"
#include "road.h"
#include "roadway.h"
#include "lane.h"
#include "connection.h"

// And your stateless calculators
#include "lanegeometrycalculator.h"
#include <qmath.h>

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

const QPainterPath &GeometryManager::roadway(const Roadway *roadway) const
{
    if (auto it = roadwayGeometryCache_.find(roadway); it != roadwayGeometryCache_.end())
    {
        return it->second;
    }
    return calculateAndCacheRoadway(roadway);
}

const QPainterPath& GeometryManager::lane(const Lane* lane) const
{
    if (auto it = laneGeometryCache_.find(lane); it != laneGeometryCache_.end())
    {
        return it->second;
    }
    return calculateAndCacheLaneGeometry(lane);
}

const QPainterPath& GeometryManager::connection(const Connection* connection) const
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
        roadwayGeometryCache_.erase(roadway);

        // Iterate through all lanes of the roadway
        for (const auto& lane : roadway->lanes())
        {
            // Erase the lane geometry from the cache
            laneGeometryCache_.erase(lane.get());

            // Iterate through all connections from that lane
            for (const auto& connection : lane->connections())
            {
                // Erase the connection geometry from the cache
                connectionGeometryCache_.erase(connection);
            }
        }
    }
}

void GeometryManager::clearAll()
{
    roadwayBaselineCache_.clear();
    roadwayGeometryCache_.clear();
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

const QPainterPath &GeometryManager::calculateAndCacheRoadway(const Roadway *roadway) const
{
    // Reuse the existing baseline calculation to get the points
    const auto& baselinePoints = roadwayBaseline(roadway);

    QPainterPath path;
    if (baselinePoints.empty())
    {
        // Return an empty path if there are no points
        return roadwayGeometryCache_.emplace(roadway, std::move(path)).first->second;
    }

    // Start the path at the first point
    path.moveTo(baselinePoints.front().position);

    // Create line segments to all subsequent points
    for (size_t i = 1; i < baselinePoints.size(); ++i)
    {
        path.lineTo(baselinePoints[i].position);
    }

    // Insert the newly created path into the cache and return it
    return roadwayGeometryCache_.emplace(roadway, std::move(path)).first->second;
}

const QPainterPath& GeometryManager::calculateAndCacheLaneGeometry(const Lane* lane) const
{
    // First, ensure the parent roadway's baseline is available (it will be calculated and cached if not)
    const auto& roadwayGeometry = roadwayBaseline(lane->roadway());
    // Now, calculate the lane geometry
    auto laneGeom = LaneGeometryCalculator::calculateGeometry(roadwayGeometry, lane, lane->roadway());
    return laneGeometryCache_.emplace(lane, std::move(laneGeom)).first->second;
}

#include <cmath> // For std::hypot

// ... inside the GeometryManager class

#include <cmath>     // For std::hypot
#include <stdexcept> // For std::logic_error

// ... inside the GeometryManager class

const QPainterPath& GeometryManager::calculateAndCacheConnectionGeometry(const Connection* connection) const
{
    const QPainterPath& sourceLaneGeom = lane(connection->source());
    const QPainterPath& destLaneGeom = lane(connection->destination());

    if (sourceLaneGeom.elementCount() < 2 || destLaneGeom.elementCount() < 2)
        throw std::runtime_error("Could not get valid lane geometry for connection calculation.");

    // --- 1. Get lane endpoints and their tangent lines ---
    QPointF sourceP1 = sourceLaneGeom.elementAt(sourceLaneGeom.elementCount() - 2);
    QPointF sourceLaneEnd = sourceLaneGeom.elementAt(sourceLaneGeom.elementCount() - 1);
    QLineF sourceTangentLine(sourceP1, sourceLaneEnd);

    QPointF destLaneStart = destLaneGeom.elementAt(0);
    QPointF destP2 = destLaneGeom.elementAt(1);
    QLineF destTangentLine(destLaneStart, destP2);

    // --- 2. Calculate the curve's offset endpoints ---
    QLineF sourceExtension(sourceP1, sourceLaneEnd);
    sourceExtension.setLength(sourceExtension.length() + connection->sourceOffset());
    QPointF curveStartPoint = sourceExtension.p2();

    QLineF destExtension(destP2, destLaneStart);
    destExtension.setLength(destExtension.length() + connection->destinationOffset());
    QPointF curveEndPoint = destExtension.p2();

    // --- 3. Build the initial path structure ---
    QPainterPath finalPath(sourceLaneEnd);
    if (connection->sourceOffset() != 0.0f) {
        finalPath.lineTo(curveStartPoint);
    }

    // --- 4. Validate Quadratic Curve by building a temporary path ---
    QPointF intersectionPoint;
    auto intersectType = sourceTangentLine.intersects(destTangentLine, &intersectionPoint);

    // Assume quadratic is valid until proven otherwise
    bool isQuadraticValid = true;
    qreal startDotProduct = 0.0;
    qreal endDotProduct = 0.0;

    if (intersectType == QLineF::NoIntersection) {
        isQuadraticValid = false;
    } else {
        // Create a temporary path *only* for the curve to check its properties
        QPainterPath tempQuadPath(curveStartPoint);
        tempQuadPath.quadTo(intersectionPoint, curveEndPoint);

        // Get the angle of the curve at its very start (0%) and very end (100%)
        qreal startAngle = tempQuadPath.angleAtPercent(0);
        qreal endAngle = tempQuadPath.angleAtPercent(1.0);

        // Create tangent vectors from these angles
        QPointF quadStartTangent = QPointF(std::cos(qDegreesToRadians(-startAngle)), std::sin(qDegreesToRadians(-startAngle)));
        QPointF quadEndTangent = QPointF(std::cos(qDegreesToRadians(-endAngle)), std::sin(qDegreesToRadians(-endAngle)));

        // Calculate dot products to check tangent validity
        startDotProduct = QPointF::dotProduct(sourceTangentLine.unitVector().p2(), quadStartTangent);
        endDotProduct = QPointF::dotProduct(destTangentLine.unitVector().p2(), quadEndTangent);

        const qreal tolerance = 0.01;
        if (startDotProduct > (1.0 - tolerance) || endDotProduct < (1.0 - tolerance)) {
            isQuadraticValid = false;
        }
    }

    // --- 5. Select Geometry based on Validation ---
    if (!isQuadraticValid)
    {
        // --- FALLBACK 1: CUBIC BEZIER ---
        const qreal connectionLength = std::hypot(curveEndPoint.x() - curveStartPoint.x(), curveEndPoint.y() - curveStartPoint.y());
        const qreal controlPointLength = connectionLength / 3.0;

        QLineF sourceControlLine(sourceP1, curveStartPoint);
        sourceControlLine.setLength(sourceControlLine.length() + controlPointLength);
        QPointF cp1 = sourceControlLine.p2();

        QLineF destControlLine(destP2, curveEndPoint);
        destControlLine.setLength(destControlLine.length() + controlPointLength);
        QPointF cp2 = destControlLine.p2();

        finalPath.cubicTo(cp1, cp2, curveEndPoint);
    }
    else
    {
        // --- PRIMARY CHOICE: QUADRATIC BEZIER ---
        finalPath.quadTo(intersectionPoint, curveEndPoint);
    }

    // --- 6. Finalize Path ---
    if (connection->destinationOffset() != 0.0f) {
        finalPath.lineTo(destLaneStart);
    }

    return connectionGeometryCache_.emplace(connection, std::move(finalPath)).first->second;
}
