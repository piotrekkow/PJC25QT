#pragma once
#include "conflictpoint.h"

class Intersection;
class GeometryManager;

class ConflictManager
{
    std::vector<std::unique_ptr<ConflictPoint>> conflictPoints_;
    std::unordered_map<const Connection*, std::vector<const ConflictPoint*>> byConnection_;
    const GeometryManager* geometry_;
    const Intersection* intersection_;

public:
    ConflictManager(const Intersection* intersection, const GeometryManager* geometry);
    void recalculate();
    const std::vector<std::unique_ptr<ConflictPoint>>& conflicts() const { return conflictPoints_; }
    const std::vector<const ConflictPoint*>& conflicts(const Connection *connection) const;
};
