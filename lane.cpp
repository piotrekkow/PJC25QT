#include "lane.h"
#include <stdexcept>
#include "intersection.h"
#include "roadway.h"

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

