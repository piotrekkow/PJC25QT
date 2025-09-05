#include "navigationstrategy.h"
#include "lane.h"
#include "traffic.h"
#include "ConflictData.h"
#include "conflictpoint.h"
#include "vehicle.h"
#include <qdebug.h>

qreal NavigationStrategy::distanceToStop() const
{
    auto conflicts = conflictsMustYieldTo();
    if (canSafelyProceed(conflicts)) {
        return std::numeric_limits<qreal>::max();
    }
    return calculateDistanceToStopLine();
}


LaneNavigationStrategy::LaneNavigationStrategy(Vehicle *vehicle, const Traffic *traffic, const GeometryManager *geometry, const Lane *lane)
    : NavigationStrategy(vehicle, traffic, geometry), lane_{ lane }
{
    designatedNextRoadway_ = traffic_->router(lane_->intersection())->route(lane_->roadway());
}

const Traversable *LaneNavigationStrategy::next() const
{
    return static_cast<const Traversable*>(nextConnection());
}

qreal LaneNavigationStrategy::distanceToStopLine()
{
    const Connection* next = nextConnection();
    return (next) ? lane_->length(geometry_) - vehicle_->progress() + next->stopLineOffset()
                  : std::numeric_limits<qreal>::max();
}

bool LaneNavigationStrategy::canSafelyProceed(const std::vector<ConflictData> &conflicts) const
{
    for (const auto& conflict : conflicts)
    {
        qreal conflictDistance = distanceToConflict(conflict.point);
        qreal approachTime = vehicle_->timeToReach(conflictDistance);

        qreal otherSpeedLimit = (conflict.point->isRHPriority(nextConnection())) ? conflict.point->yieldConnection()->speedLimit()
                                                                               : conflict.point->priorityConnection()->speedLimit();

        // percieved other vehicle conflict times
        for (const auto& other : conflict.priorityAgents)
        {
            qreal otherApproachTime = other.distanceToConflictPoint / otherSpeedLimit;
            // TODO: Don't check other vehicles in queue
            // TODO: When speed limiting for curves is implemented might mess this up

            if (std::abs(approachTime - otherApproachTime) < minTimeGap_)
            {
                // Unsafe gap, we must wait.
                return false;
            }
        }
    }
    return true;
}

const std::vector<ConflictData> LaneNavigationStrategy::conflictsMustYieldTo() const
{
    auto controller = traffic_->controller(lane_->intersection());
    return controller->conflictsMustYieldTo(nextConnection(), traffic_->agents(), geometry_);
}

qreal LaneNavigationStrategy::calculateDistanceToStopLine() const
{
    const Connection* next = nextConnection();
    return (next) ? lane_->length(geometry_) - vehicle_->progress() + next->stopLineOffset()
                  : std::numeric_limits<qreal>::max();
}

qreal LaneNavigationStrategy::distanceToConflict(const ConflictPoint* cp) const
{
    return lane_->length(geometry_) - vehicle_->progress() + cp->distanceFrom(nextConnection());
}

const Connection *LaneNavigationStrategy::nextConnection() const
{
    for (const auto& connection : lane_->nextConnections())
    {
        if (connection->nextLane()->roadway() == designatedNextRoadway_)
            return connection;
    }
    return nullptr;
}

const Traversable *ConnectionNavigationStrategy::next() const
{
    return static_cast<const Traversable*>(connection_->nextLane());
}

qreal ConnectionNavigationStrategy::distanceToStopLine()
{
    // TODO: calculate next stop line rather than outputing negative, may improve navigation if next intersection is very close
    return connection_->stopLineOffset() - vehicle_->progress();
}

bool ConnectionNavigationStrategy::canSafelyProceed(const std::vector<ConflictData> &conflicts) const
{
    for (const auto& conflict : conflicts)
    {
        qreal conflictDistance = distanceToConflict(conflict.point);
        qreal approachTime = vehicle_->timeToReach(conflictDistance);

        qreal otherSpeedLimit = (conflict.point->isRHPriority(connection_)) ? conflict.point->yieldConnection()->speedLimit()
                                                                          : conflict.point->priorityConnection()->speedLimit();

        // percieved other vehicle conflict times
        for (const auto& other : conflict.priorityAgents)
        {
            qreal otherApproachTime = other.distanceToConflictPoint / otherSpeedLimit;
            // TODO: Don't check other vehicles in queue
            // TODO: When speed limiting for curves is implemented might mess this up

            if (std::abs(approachTime - otherApproachTime) < minTimeGap_)
            {
                // Unsafe gap, we must wait.
                return false;
            }
        }
    }
    return true;
}

const std::vector<ConflictData> ConnectionNavigationStrategy::conflictsMustYieldTo() const
{
    auto controller = traffic_->controller(connection_->intersection());
    return controller->conflictsMustYieldTo(connection_, traffic_->agents(), geometry_);
}

qreal ConnectionNavigationStrategy::calculateDistanceToStopLine() const
{
    return connection_->stopLineOffset() - vehicle_->progress();
}

qreal ConnectionNavigationStrategy::distanceToConflict(const ConflictPoint *cp) const
{
    return cp->distanceFrom(connection_) - vehicle_->progress();
}


