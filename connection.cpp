#include "connection.h"
#include "geometrymanager.h"
#include "roadway.h"
#include "conflictmanager.h"

Connection::Connection(Lane *source, Lane *destination)
    : sourceLane_{ source }
    , destinationLane_{ destination }
    , sourceOffset_{ 0.0 }
    , destinationOffset_{ 0.0 }
    , stopLineOffset_{ 0.0 }
{
}

PriorityType Connection::roadwayPriority() const
{
    return sourceLane_->roadway()->priority();
}

const Intersection *Connection::intersection() const
{
    return sourceLane_->roadway()->destination();
}

bool Connection::stopLineOffset(qreal offset, const ConflictManager *conflictManager)
{
    if (offset < 0.0) return false;

    qreal firstConflictDistance = std::numeric_limits<qreal>::max();
    for (const auto& conflict : conflictManager->conflicts(this))
    {
        if (conflict->classify() != ConflictPoint::ConflictType::Diverging)
        {
            firstConflictDistance = std::min(firstConflictDistance, conflict->distanceFrom(this));
        }
    }

    if (offset >= firstConflictDistance) return false;

    stopLineOffset_ = offset;
    return true;
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
