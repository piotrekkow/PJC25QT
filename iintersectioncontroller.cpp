#include "iintersectioncontroller.h"
#include "intersection.h"
#include "lane.h"
#include "vehicle.h"
#include "IntersectionDecisionData.h"
#include "DebugHelpers.h"

const IntersectionDecisionData UncontrolledIntersection::decisionData(const Vehicle *vehicle, const std::vector<std::unique_ptr<Vehicle> > &allVehicles) const
{
    auto* traversable = vehicle->traversable();
    if (traversable->intersection() != intersection_) throw std::invalid_argument("Vehicle making decision at an invalid intersection");

    if (traversable->type() == ITraversable::TraversableType::Connection)
    {
        auto* connection = static_cast<Connection*>(traversable);
        qreal distanceToStopLine = connection->stopLineOffset() - vehicle->progress();
        if (distanceToStopLine >= 0)
            return conflictsMustYieldTo(connection, allVehicles);
    }

    if (traversable->type() == ITraversable::TraversableType::Lane)
    {
        Lane* lane = static_cast<Lane*>(traversable);
        if (lane->next().empty()) return { false, {} };
        Connection* nextConnection = static_cast<Connection*>(lane->next()[0]); // for now we take the first available connection, when pathfinding is added this will change

        qreal distanceToStopLine = lane->length(geometry_) + nextConnection->stopLineOffset() - vehicle->progress();
        if (distanceToStopLine <= vehicle->decisionDistance())
        {
            return conflictsMustYieldTo(nextConnection, allVehicles);
        }
    }

    return { false, {} };    // can proceed, past stop line
}

bool UncontrolledIntersection::mustYieldTo(const Connection *ourConn, const ConflictPoint *cp) const
{
    if (cp->isPriority(ourConn)) return ourConn->roadwayPriority() < cp->yieldConnection()->roadwayPriority();
    else return ourConn->roadwayPriority() <= cp->priorityConnection()->roadwayPriority();
}

const IntersectionDecisionData UncontrolledIntersection::conflictsMustYieldTo(const Connection *conn, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) const
{
    IntersectionDecisionData decisionData;
    decisionData.mustPerformFullStop = (conn->roadwayPriority() == PriorityType::Stop);

    for (const auto& cp : intersection_->conflictManager()->conflicts(conn))
    {
        if (cp->classify() == ConflictPoint::ConflictType::Diverging) continue;
        if (!mustYieldTo(conn, cp)) continue;

        // here priorityConn meaning having higher priority level as deduced above
        // cp->priorityConnection, priority when compared to other member (right hand rule)
        const Connection* priorityConn = (cp->priorityConnection() == conn)
                                             ? cp->yieldConnection()
                                             : cp->priorityConnection();

        std::vector<PriorityVehicleInfo> foundVehicles;

        for (const auto& vehicle : allVehicles)
        {
            const auto* traversable = vehicle->traversable();

            // filter irrelevant vehicles (not on conflict connection or approaching lane)
            if (traversable != priorityConn && traversable != priorityConn->source()) continue;

            qreal distance = (traversable == priorityConn)
                                ? cp->distanceFrom(priorityConn) - vehicle->progress()
                                : priorityConn->source()->length(geometry_) - vehicle->progress() + cp->distanceFrom(priorityConn);

            foundVehicles.emplace_back(PriorityVehicleInfo{vehicle.get(), distance});
        }

        if (!foundVehicles.empty()) decisionData.conflictsToEvaluate.emplace_back(ConflictData{cp, foundVehicles});
    }

    qDebug() << "decisionData:" << decisionData;
    return decisionData;
}
