#pragma once

#include <QDebug>
#include "ConflictData.h"
#include "conflictpoint.h"

/**
 *  ConflictData debug overloads
*/

// 1. Overload for PriorityAgentInfo
QDebug operator<<(QDebug debug, const PriorityAgentInfo& info) {
    // QDebugStateSaver ensures that formatting changes (like nospace) are local to this function.
    QDebugStateSaver saver(debug);
    debug.nospace() << "PriorityAgentInfo(Agent: " << info.agent    // TODO: type deduction "Agent: "
                    << ", Dist: " << info.distanceToConflictPoint << "m)";
    return debug;
}

// 2. Overload for ConflictData
QDebug operator<<(QDebug debug, const ConflictData& data) {
    QDebugStateSaver saver(debug);
    // Let QDebug handle indentation and newlines automatically with maybeSpace() and maybeNewline()
    debug.nospace() << "ConflictData(Point: " << data.point->position().x() << ", " << data.point->position().y();
    debug.maybeSpace() << "Agents: " << data.priorityAgents << ")";
    return debug;
}

// 3. Overload for the top-level ConflictData
QDebug operator<<(QDebug debug, const std::vector<ConflictData>& conflicts) {
    QDebugStateSaver saver(debug);
    // .noquote() is useful if you are logging to a text file and don't want extra quotes.
    // .space() puts spaces between items, .nospace() doesn't.
    debug.nospace() << "Conflicts: ";

    // QDebug can print a QVector/std::vector directly!
    // It will use our overloaded operators for the elements.
    if (conflicts.empty()) {
        debug.nospace() << "[None]\n";
    } else {
        // For better formatting of a list of complex types, a manual loop is often clearer.
        debug.nospace() << "[\n";
        for (const auto& conflict : conflicts) {
            debug.nospace() << "    " << conflict << "\n";
        }
        debug.nospace() << "  ]\n";
    }

    debug.nospace() << "}";
    return debug;
}
