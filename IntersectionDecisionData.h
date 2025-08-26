#pragma once
#include <vector>
#include <qtypes.h>

class Vehicle;
class ConflictPoint;

/**
 * @brief Raw, objective information about a priority vehicle relative to a conflict.
 */
struct PriorityVehicleInfo {
    /// A pointer to the vehicle that has the right-of-way.
    const Vehicle* vehicle;

    /// The physical distance (in meters) along the priority vehicle's path
    /// from its current position to the conflict point.
    qreal distanceToConflictPoint;
};

/**
 * @brief A collection of threats related to a single point of conflict.
 */
struct ConflictData {
    /// The geometric conflict point being evaluated.
    const ConflictPoint* point;

    /// A list of all priority vehicles that pose a potential threat at this point.
    /// This may be empty if the way is clear.
    std::vector<PriorityVehicleInfo> priorityVehicles;
};

/**
 * @brief The complete situational awareness data packet provided by a controller
 * to a vehicle to allow it to make its own decision.
 */
struct IntersectionDecisionData {
    /// True if a static rule (e.g., STOP sign) dictates the vehicle must
    /// perform a full stop before evaluating dynamic conflicts.
    bool mustPerformFullStop;

    /// A list of all dynamic conflicts the vehicle must consider before proceeding.
    std::vector<ConflictData> conflictsToEvaluate;
};
