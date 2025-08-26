#pragma once

#include <QDebug>
#include "IntersectionDecisionData.h"

/**
 *  IntersectionDecisionData debug overloads
*/

// 1. Overload for PriorityVehicleInfo
QDebug operator<<(QDebug debug, const PriorityVehicleInfo& info) {
    // QDebugStateSaver ensures that formatting changes (like nospace) are local to this function.
    QDebugStateSaver saver(debug);
    debug.nospace() << "PriorityVehicleInfo(Vehicle: " << info.vehicle
                    << ", Dist: " << info.distanceToConflictPoint << "m)";
    return debug;
}

// 2. Overload for ConflictData
QDebug operator<<(QDebug debug, const ConflictData& data) {
    QDebugStateSaver saver(debug);
    // Let QDebug handle indentation and newlines automatically with maybeSpace() and maybeNewline()
    debug.nospace() << "ConflictData(Point: " << data.point;
    debug.maybeSpace() << "Vehicles: " << data.priorityVehicles << ")";
    return debug;
}

// 3. Overload for the top-level IntersectionDecisionData
QDebug operator<<(QDebug debug, const IntersectionDecisionData& data) {
    QDebugStateSaver saver(debug);
    // .noquote() is useful if you are logging to a text file and don't want extra quotes.
    // .space() puts spaces between items, .nospace() doesn't.
    debug.nospace() << "IntersectionDecisionData {\n"
                    << "  Must Stop: " << data.mustPerformFullStop << "\n"
                    << "  Conflicts: ";

    // QDebug can print a QVector/std::vector directly!
    // It will use our overloaded operators for the elements.
    if (data.conflictsToEvaluate.empty()) {
        debug.nospace() << "[None]\n";
    } else {
        // For better formatting of a list of complex types, a manual loop is often clearer.
        debug.nospace() << "[\n";
        for (const auto& conflict : data.conflictsToEvaluate) {
            debug.nospace() << "    " << conflict << "\n";
        }
        debug.nospace() << "  ]\n";
    }

    debug.nospace() << "}";
    return debug;
}
