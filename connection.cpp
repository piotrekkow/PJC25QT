#include "connection.h"
#include "geometrymanager.h"
#include "conflictmanager.h"
#include "lane.h"
#include "navigationstrategy.h"

Connection::Connection(const Lane *source, const Lane *destination)
    : sourceLane_{ source }
    , destinationLane_{ destination }
    , sourceOffset_{ 0.0 }
    , destinationOffset_{ 0.0 }
    , stopLineOffset_{ 0.0 }
{}

const QPainterPath &Connection::path(const GeometryManager *geometryManager) const
{
    return geometryManager->connection(this);
}

qreal Connection::length(const GeometryManager *geometryManager) const
{
    return geometryManager->connection(this).length();
}

std::vector<const Traversable *> Connection::next() const
{
    return { static_cast<const Traversable*>(destinationLane_) };
}

PriorityType Connection::regulatoryPriority() const
{
    return sourceLane_->regulatoryPriority();
}

qreal Connection::speedLimit() const
{
    return sourceLane_->speedLimit();
}

const Intersection *Connection::intersection() const
{
    return sourceLane_->intersection();
}

std::unique_ptr<NavigationStrategy> Connection::Connection::createNavigationStrategy(Vehicle *vehicle, const Traffic *traffic, const GeometryManager *geometry) const
{
    return std::make_unique<ConnectionNavigationStrategy>(
        vehicle, traffic, geometry, this
        );
}


