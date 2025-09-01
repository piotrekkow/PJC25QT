#include "lane.h"
#include "roadway.h"
#include "geometrymanager.h"
#include "navigationstrategy.h"

void Lane::addConnection(const Connection *connection)
{
    if (!connection) return;
    connections_.emplace_back(connection);
}

const QPainterPath &Lane::path(const GeometryManager *geometryManager) const
{
    return geometryManager->lane(this);
}

qreal Lane::length(const GeometryManager *geometryManager) const
{
    return geometryManager->lane(this).length();
}

std::vector<const Traversable *> Lane::next() const
{
    std::vector<const Traversable*> next;
    next.reserve(connections_.size());
    for (auto& c : connections_)
    {
        next.push_back(static_cast<const Traversable*>(c));
    }
    return next;
}

PriorityType Lane::regulatoryPriority() const
{
    return roadway_->priority();
}

qreal Lane::speedLimit() const
{
    return roadway_->speedLimit();
}

const Intersection *Lane::intersection() const
{
    return roadway_->destination();
}

std::unique_ptr<NavigationStrategy> Lane::createNavigationStrategy(Vehicle *vehicle, const Traffic *traffic, const GeometryManager *geometry) const
{
    return std::make_unique<LaneNavigationStrategy>(
        vehicle, traffic, geometry, this
        );
}
