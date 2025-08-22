#include "conflictmanager.h"
#include "intersection.h"
#include "geometrymanager.h"
#include "conflictgeometrycalculator.h"
#include "qmath.h"

ConflictManager::ConflictManager(const Intersection *intersection, const GeometryManager *geometry)
    : geometry_{ geometry }
    , intersection_{ intersection }
{}

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


// void ConflictManager::recalculate() {
//     conflictPoints_.clear();
//     byConnection_.clear();

//     const auto& connections = intersection_->connections();
//     for (size_t i = 0; i < connections.size(); ++i) {
//         for (size_t j = i + 1; j < connections.size(); ++j) // offset inner loop to check only unique pairs (avoiding cp(a,b), cp(b,a) duplicates
//         {
//             const auto* a = connections[i].get();
//             const auto* b = connections[j].get();

//             // if (a->source() == b->source()) continue; // keep or revise based on your policy

//             const auto& pathA = geometry_->connection(a);
//             const auto& pathB = geometry_->connection(b);

//             // Fast reject:
//             if (!pathA.boundingRect().intersects(pathB.boundingRect())) continue;

//             auto cps = ConflictGeometryCalculator::calculatePathIntersectionPoints(pathA, pathB, /*tolerance*/ 0.05f);
//             for (const auto& cp : cps) {
//                 auto ptr = std::make_unique<ConflictPoint>(a, b, cp.point, cp.distanceAlongPath1, cp.distanceAlongPath2);
//                 auto raw = ptr.get();
//                 conflictPoints_.push_back(std::move(ptr));
//                 byConnection_[a].push_back(raw);
//                 byConnection_[b].push_back(raw);
//             }
//         }
//     }
// }

const std::vector<const ConflictPoint*>& ConflictManager::conflicts(Connection *connection) const
{
    static const std::vector<const ConflictPoint*> empty;
    auto it = byConnection_.find(connection);
    return it == byConnection_.end() ? empty : it->second;
}

