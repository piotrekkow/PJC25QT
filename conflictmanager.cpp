#include "conflictmanager.h"
#include "intersection.h"
#include "geometrymanager.h"
#include "conflictgeometrycalculator.h"
#include "qmath.h"

ConflictManager::ConflictManager(const Intersection *intersection, const GeometryManager *geometry)
    : geometry_{ geometry }
    , intersection_{ intersection }
{}

namespace {
// Helper function to get the incoming tangent of a connection's source lane
QLineF getIncomingTangent(const Connection* connection, const GeometryManager* geometry)
{
    const auto& path = geometry->lane(connection->source());
    if (path.elementCount() < 2)
    {
        return {};
    }
    // Use the last segment of the lane path to determine the incoming direction
    QPointF p1 = path.pointAtPercent(0.99);
    QPointF p2 = path.pointAtPercent(1.0);
    return QLineF(p1, p2);
}
}

void ConflictManager::recalculate()
{
    conflictPoints_.clear();
    byConnection_.clear();

    const auto& connections = intersection_->connections();
    for (size_t i = 0; i < connections.size(); ++i)
    {
        for (size_t j = i + 1; j < connections.size(); ++j)
        {
            const auto* connA = connections[i].get();
            const auto* connB = connections[j].get();

            // if (connA->source() == connB->source()) continue;    // optionally dont calculate diverging conflict points

            const Connection* priorityConnection = connA;
            const Connection* yieldConnection = connB;

            // --- Determine Priority using "Priority to the Right" rule ---
            QLineF tangentA = getIncomingTangent(connA, geometry_);
            QLineF tangentB = getIncomingTangent(connB, geometry_);

            // Use the 2D cross product of the direction vectors to determine relative orientation.
            // A positive result means tangentB is to the left of tangentA.
            qreal crossProduct = tangentA.dx() * tangentB.dy() - tangentA.dy() * tangentB.dx();

            if (crossProduct > 0)
            {
                // connB is to the right of connA, so connB has priority.
                priorityConnection = connB;
                yieldConnection = connA;
            }

            // --- REVISED: Special handling for merging conflicts ---
            if (connA->destination() == connB->destination())
            {
                const auto& destLanePath = geometry_->lane(priorityConnection->destination());
                if (destLanePath.isEmpty()) continue;

                QPointF destLaneStart = destLanePath.pointAtPercent(0.0);
                qreal startAngle = destLanePath.angleAtPercent(0.0);

                qreal angleRad = qDegreesToRadians(-startAngle);
                QPointF tangentVector(std::cos(angleRad), std::sin(angleRad));
                QLineF destTangent(destLaneStart, destLaneStart + tangentVector);

                qreal minOffset = std::min(priorityConnection->destinationOffset(), yieldConnection->destinationOffset());

                destTangent.setLength(-minOffset);
                QPointF conflictPos = destTangent.p2();

                qreal distPriority = geometry_->connection(priorityConnection).length() - (priorityConnection->destinationOffset() - minOffset);
                qreal distYield = geometry_->connection(yieldConnection).length() - (yieldConnection->destinationOffset() - minOffset);

                auto ptr = std::make_unique<ConflictPoint>(priorityConnection, yieldConnection, conflictPos, distPriority, distYield);
                auto raw = ptr.get();
                conflictPoints_.push_back(std::move(ptr));
                byConnection_[priorityConnection].push_back(raw);
                byConnection_[yieldConnection].push_back(raw);
            }
            else // Use the existing intersection logic for crossing conflicts
            {
                const auto& pathA = geometry_->connection(priorityConnection);
                const auto& pathB = geometry_->connection(yieldConnection);

                if (!pathA.boundingRect().intersects(pathB.boundingRect())) continue;

                auto cps = ConflictGeometryCalculator::calculatePathIntersectionPoints(pathA, pathB, 0.05f);
                for (const auto& cp : cps)
                {
                    auto ptr = std::make_unique<ConflictPoint>(priorityConnection, yieldConnection, cp.point, cp.distanceAlongPath1, cp.distanceAlongPath2);
                    auto raw = ptr.get();
                    conflictPoints_.push_back(std::move(ptr));
                    byConnection_[priorityConnection].push_back(raw);
                    byConnection_[yieldConnection].push_back(raw);
                }
            }
        }
    }
}

/*
void ConflictManager::recalculate() {
    conflictPoints_.clear();
    byConnection_.clear();

    const auto& connections = intersection_->connections();
    for (size_t i = 0; i < connections.size(); ++i) {
        for (size_t j = i + 1; j < connections.size(); ++j)
        {
            const auto* a = connections[i].get();
            const auto* b = connections[j].get();

            // if (a->source() == b->source()) continue;

            // --- REVISED: Special handling for merging conflicts ---
            if (a->destination() == b->destination())
            {
                const auto& destLanePath = geometry_->lane(a->destination());
                if (destLanePath.isEmpty()) continue;

                // 1. Get the starting point and tangent angle of the destination lane.
                //    This is robust and works for both lines and curves.
                QPointF destLaneStart = destLanePath.pointAtPercent(0.0);
                qreal startAngle = destLanePath.angleAtPercent(0.0); // Angle in degrees, counter-clockwise

                // 2. Create a tangent vector pointing *away* from the lane's direction.
                //    We negate the angle and convert to radians for standard trigonometric functions.
                qreal angleRad = qDegreesToRadians(-startAngle);
                QPointF tangentVector(std::cos(angleRad), std::sin(angleRad));
                QLineF destTangent(destLaneStart, destLaneStart + tangentVector);

                // 3. Find the minimum destination offset to locate the conflict point.
                qreal minOffset = std::min(a->destinationOffset(), b->destinationOffset());

                // 4. Calculate the precise conflict position along the reversed tangent.
                destTangent.setLength(-minOffset);
                QPointF conflictPos = destTangent.p2();

                // 5. Calculate the distance along each connection's path to this conflict point.
                qreal distA = geometry_->connection(a).length() - (a->destinationOffset() - minOffset);
                qreal distB = geometry_->connection(b).length() - (b->destinationOffset() - minOffset);

                auto ptr = std::make_unique<ConflictPoint>(a, b, conflictPos, distA, distB);
                auto raw = ptr.get();
                conflictPoints_.push_back(std::move(ptr));
                byConnection_[a].push_back(raw);
                byConnection_[b].push_back(raw);
            }
            else // Use the existing intersection logic for crossing conflicts
            {
                const auto& pathA = geometry_->connection(a);
                const auto& pathB = geometry_->connection(b);

                if (!pathA.boundingRect().intersects(pathB.boundingRect())) continue;

                auto cps = ConflictGeometryCalculator::calculatePathIntersectionPoints(pathA, pathB, 0.05f);
                for (const auto& cp : cps) {
                    auto ptr = std::make_unique<ConflictPoint>(a, b, cp.point, cp.distanceAlongPath1, cp.distanceAlongPath2);
                    auto raw = ptr.get();
                    conflictPoints_.push_back(std::move(ptr));
                    byConnection_[a].push_back(raw);
                    byConnection_[b].push_back(raw);
                }
            }
        }
    }
}

*/

const std::vector<const ConflictPoint*>& ConflictManager::conflicts(const Connection *connection) const
{
    static const std::vector<const ConflictPoint*> empty;
    auto it = byConnection_.find(connection);
    return it == byConnection_.end() ? empty : it->second;
}

