#pragma once
#include "conflictpoint.h"

class Intersection;
class GeometryManager;

/**
 * @brief Calculates conflict point characteristics and manages relevant conflict point objects at an intersection
 */
class ConflictManager
{
    std::vector<std::unique_ptr<ConflictPoint>> conflictPoints_;
    std::unordered_map<const Connection*, std::vector<const ConflictPoint*>> byConnection_;
    const GeometryManager* geometry_;
    const Intersection* intersection_;

public:
    ConflictManager(const Intersection* intersection, const GeometryManager* geometry);

    /**
     * @brief recalculates all conflict points at intersection using existing connections
     */
    void recalculate();

    /**
     * @return returns all conflict points at an intersection
     */
    const std::vector<std::unique_ptr<ConflictPoint>>& conflicts() const { return conflictPoints_; }

    /**
     * @param connection
     * @return returns all conflict points on connection
     */
    const std::vector<const ConflictPoint*>& conflicts(const Connection *connection) const;
};
