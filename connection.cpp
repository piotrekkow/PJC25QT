#include "connection.h"
#include "geometrymanager.h"
#include "lane.h"

Connection::Connection(Lane *source, Lane *destination)
    : sourceLane_{ source }
    , destinationLane_{ destination }
    , sourceOffset_{ 0.0f }
    , destinationOffset_{ 0.0f }
{
}

const QPainterPath &Connection::path(const GeometryManager *geometryManager) const
{
    return geometryManager->connection(this);
}

qreal Connection::length(const GeometryManager *geometryManager) const
{
    return geometryManager->connection(this).length();
}

std::vector<ITraversable *> Connection::next() const
{
    return { static_cast<ITraversable*>(destinationLane_) };
}
