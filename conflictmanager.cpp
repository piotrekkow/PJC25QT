#include "conflictmanager.h"
#include "intersection.h"
#include "geometrymanager.h"
#include "conflictgeometrycalculator.h"
#include "qmath.h"

ConflictManager::ConflictManager(const Intersection *intersection, const GeometryManager *geometry)
    : geometry_{ geometry }
    , intersection_{ intersection }
{}

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

            // Get the geometry paths for both connections
            const QPainterPath& pathA = geometry_->connection(connA);
            const QPainterPath& pathB = geometry_->connection(connB);

            if (pathA.elementCount() < 1 || pathB.elementCount() < 1) continue;

            // Create vectors from the start to the end of each connection's path
            QPointF vectorA = { pathA.elementAt(pathA.elementCount() - 1).x - pathA.elementAt(0).x,
                                pathA.elementAt(pathA.elementCount() - 1).y - pathA.elementAt(0).y };
            QPointF vectorB = { pathB.elementAt(pathB.elementCount() - 1).x - pathB.elementAt(0).x,
                                pathB.elementAt(pathB.elementCount() - 1).y - pathB.elementAt(0).y };

            // A negative result means vectorB is to the right of vectorA.
            qreal crossProduct = vectorA.x() * vectorB.y() - vectorA.y() * vectorB.x();

            if (crossProduct < 0)
            {
                // connB's path is to the right of connA's path, so connB has priority.
                priorityConnection = connB;
                yieldConnection = connA;
            }

            // Special handling for merging conflicts
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
            else // logic for crossing conflicts
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

const std::vector<const ConflictPoint*>& ConflictManager::conflicts(const Connection *connection) const
{
    static const std::vector<const ConflictPoint*> empty;
    auto it = byConnection_.find(connection);
    return it == byConnection_.end() ? empty : it->second;
}

