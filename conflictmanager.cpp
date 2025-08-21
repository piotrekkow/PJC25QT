#include "conflictmanager.h"
#include "intersection.h"
#include "geometrymanager.h"
#include "conflictgeometrycalculator.h"

ConflictManager::ConflictManager(const Intersection *intersection, const GeometryManager *geometry)
    : geometry_{ geometry }
    , intersection_{ intersection }
{}

void ConflictManager::recalculate() {
    conflictPoints_.clear();
    byConnection_.clear();

    const auto& connections = intersection_->connections();
    for (size_t i = 0; i < connections.size(); ++i) {
        for (size_t j = i + 1; j < connections.size(); ++j) // offset inner loop to check only unique pairs (avoiding cp(a,b), cp(b,a) duplicates
        {
            const auto* a = connections[i].get();
            const auto* b = connections[j].get();

            if (a->source() == b->source()) continue; // keep or revise based on your policy

            const auto& pathA = geometry_->connection(a);
            const auto& pathB = geometry_->connection(b);

            // Fast reject:
            if (!pathA.boundingRect().intersects(pathB.boundingRect())) continue;

            auto cps = ConflictGeometryCalculator::calculatePathIntersectionPoints(pathA, pathB, /*tolerance*/ 0.05f);
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

const std::vector<const ConflictPoint*>& ConflictManager::conflicts(Connection *connection) const
{
    static const std::vector<const ConflictPoint*> empty;
    auto it = byConnection_.find(connection);
    return it == byConnection_.end() ? empty : it->second;
}

