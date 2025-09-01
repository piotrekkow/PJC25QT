#include "intersection.h"
#include "road.h"

Intersection::Intersection(QPointF position, GeometryManager* geometry)
    : position_{ position }
{
    conflictManager_ = std::make_unique<ConflictManager>(this, geometry);
}

void Intersection::addRoad(Road *road)
{
    roads_.emplace_back(road);
}

Connection* Intersection::createConnection(Lane *source, Lane *destination)
{
    if (!source || !destination)
        throw std::invalid_argument("Cannot create a connection with null lanes.");

    if (this != source->roadway()->destination())
        throw std::invalid_argument("Source lane does not lead into this intersection.");

    if (this != destination->roadway()->source())
        throw std::invalid_argument("Destination lane does not originate from this intersection.");

    auto newConnection = std::make_unique<Connection>(source, destination);
    Connection* connectionPtr = newConnection.get();

    source->addConnection(connectionPtr);
    connections_.emplace_back(std::move(newConnection));

    return connectionPtr;
}
