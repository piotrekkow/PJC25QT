#pragma once

#include <QDebug>
#include "ConflictData.h"
#include "conflictpoint.h"

/**
 *  ConflictData debug overloads
*/

// 1. Overload for PriorityAgentInfo
QDebug operator<<(QDebug debug, const PriorityAgentInfo& info) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "PriorityAgentInfo(Agent: " << info.agent    // TODO: type deduction "Agent: "
                    << ", Dist: " << info.distanceToConflictPoint << "m)";
    return debug;
}

// 2. Overload for ConflictData
QDebug operator<<(QDebug debug, const ConflictData& data) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "ConflictData(Point: " << data.point->position().x() << ", " << data.point->position().y();
    debug.maybeSpace() << "Agents: " << data.priorityAgents << ")";
    return debug;
}

// 3. Overload for the top-level ConflictData
QDebug operator<<(QDebug debug, const std::vector<ConflictData>& conflicts) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "Conflicts: ";

    if (conflicts.empty()) {
        debug.nospace() << "[None]\n";
    } else {
        debug.nospace() << "[\n";
        for (const auto& conflict : conflicts) {
            debug.nospace() << "    " << conflict << "\n";
        }
        debug.nospace() << "  ]\n";
    }

    debug.nospace() << "}";
    return debug;
}
