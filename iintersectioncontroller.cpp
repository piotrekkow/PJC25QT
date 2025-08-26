#include "iintersectioncontroller.h"
#include "intersection.h"
#include "lane.h"
#include "vehicle.h"
#include "IntersectionDecisionData.h"
#include "DebugHelpers.h"

const IntersectionDecisionData UncontrolledIntersection::decisionData(const Vehicle *vehicle, const std::vector<std::unique_ptr<Vehicle> > &allVehicles) const
{
    auto* vehiclesTraversable = vehicle->traversable();  
    if (vehiclesTraversable->type() == ITraversable::TraversableType::Connection)
    {
        auto* connection = static_cast<Connection*>(vehiclesTraversable);
        qreal distanceToStopLine = connection->stopLineOffset() - vehicle->progress();
        if (distanceToStopLine >= 0)
            return conflictsMustYieldTo(connection, allVehicles);
    }

    if (vehiclesTraversable->type() == ITraversable::TraversableType::Lane)
    {
        Lane* lane = static_cast<Lane*>(vehiclesTraversable);
        Connection* nextConnection = static_cast<Connection*>(lane->next()[0]); // for now we take the first available connection, when pathfinding is added this will change

        qreal distanceToStopLine = lane->length(geometry_) + nextConnection->stopLineOffset() - vehicle->progress();
        if (distanceToStopLine <= vehicle->decisionDistance())
            return conflictsMustYieldTo(nextConnection, allVehicles);
    }

    return { false, {} };    // can proceed, past stop line
}

// bool UncontrolledIntersection::mustYieldTo(const Connection *self, const Connection *other)
// {
//     // Diverging conflict point case
//     if (self->source() == other->source()) return false;

//     // Lower priority level gives way to higher priority level
//     if (self->roadwayPriority() > other->roadwayPriority()) return true;
//     if (self->roadwayPriority() < other->roadwayPriority()) return false;

//     // Same priority level - check for priority to the right rule baked into connections
//     const auto& conflictPoints = intersection_->conflictManager()->conflicts(self);
//     auto result = std::ranges::find_if(conflictPoints,
//                                        [&](const ConflictPoint* cp) {
//                                            // The predicate: find the conflict point that involves 'other'.
//                                            // We already know 'self' is involved because of the initial query.
//                                            return cp->priorityConnection() == other || cp->yieldConnection() == other;
//                                        }
//                                        );

//     if (result != conflictPoints.end())
//     {
//         // A conflict was found. Dereference the iterator to get the pointer.
//         const ConflictPoint* theConflict = *result;

//         // Now, check who has priority *at this specific conflict point*.
//         // 'self' must yield if it is the 'yieldConnection' in this conflict.
//         return theConflict->yieldConnection() == self;
//     }

//     // No conflict exists between self and other
//     return false;
// }

const IntersectionDecisionData UncontrolledIntersection::conflictsMustYieldTo(const Connection *conn, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) const
{
    IntersectionDecisionData decisionData;
    for (const auto& cp : intersection_->conflictManager()->conflicts(conn))
    {
        if (cp->classify() == ConflictPoint::ConflictType::Diverging) continue;

        if (cp->isPriority(conn))
        {
            if (conn->roadwayPriority() >= cp->yieldConnection()->roadwayPriority()) continue; // roadway priority (regulatory signs) takes precedence over conflict point priority (right hand rule)

            ConflictData cpData;

            for (const auto& vehicle : allVehicles)
            {
                // filter irrelevant vehicles (not on conflict connection or approaching lane)
                const ITraversable* priorityConn = static_cast<const ITraversable*>(cp->yieldConnection());
                const ITraversable* sourceLane = static_cast<const ITraversable*>(cp->yieldConnection()->source());

                if (vehicle->traversable() != priorityConn && vehicle->traversable() != sourceLane) continue;

                PriorityVehicleInfo vehicleInfo;
                vehicleInfo.distanceToConflictPoint = cp->distanceFromYield() - vehicle->progress();
                vehicleInfo.vehicle = vehicle.get();

                cpData.priorityVehicles.emplace_back(vehicleInfo);
            }
            cpData.point = cp;
            decisionData.conflictsToEvaluate.emplace_back(cpData);
        }
        else
        {
            if (conn->roadwayPriority() > cp->priorityConnection()->roadwayPriority()) continue;

            ConflictData cpData;

            for (const auto& vehicle : allVehicles)
            {
                // filter irrelevant vehicles (not on conflict connection or approaching lane)
                const ITraversable* priorityConn = static_cast<const ITraversable*>(cp->priorityConnection());
                const ITraversable* sourceLane = static_cast<const ITraversable*>(cp->priorityConnection()->source());

                if (vehicle->traversable() != priorityConn && vehicle->traversable() != sourceLane) continue;

                PriorityVehicleInfo vehicleInfo;
                vehicleInfo.distanceToConflictPoint = cp->distanceFromYield() - vehicle->progress();
                vehicleInfo.vehicle = vehicle.get();

                cpData.priorityVehicles.emplace_back(vehicleInfo);
            }
            cpData.point = cp;
            decisionData.conflictsToEvaluate.emplace_back(cpData);
        }
    }
    if (conn->roadwayPriority() == PriorityType::Stop)
        decisionData.mustPerformFullStop = true;
    else
        decisionData.mustPerformFullStop = false;

    qDebug() << "decisionData:" << decisionData;
    return decisionData;
}
