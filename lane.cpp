#include "lane.h"
#include "roadway.h"
#include "geometrymanager.h"

Lane::Lane(Roadway* parent, std::optional<qreal> length, qreal width)
    : width_{ width }
    , length_{ length }
    , roadway_{ parent }
{
}

void Lane::addOutgoingConnection(Connection *connection)
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

std::vector<ITraversable *> Lane::next() const
{
    std::vector<ITraversable*> nextTraversables;
    nextTraversables.reserve(connections_.size());
    for (auto& c : connections_)
    {
        nextTraversables.push_back(static_cast<ITraversable*>(c));
    }
    return nextTraversables;
}

