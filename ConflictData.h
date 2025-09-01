#pragma once
#include <vector>
#include <qtypes.h>

class Agent;
class ConflictPoint;

/**
 * @brief Raw, objective information about a priority vehicle relative to a conflict.
 */
struct PriorityAgentInfo {
    /// A pointer to the vehicle that has the right-of-way.
    const Agent* agent;

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

    /// A list of all priority agents that pose a potential threat at this point.
    /// This may be empty if the way is clear.
    std::vector<PriorityAgentInfo> priorityAgents;
};
