#include "intersection.h"
#include "road.h"

Intersection::Intersection(QPointF position, GeometryManager* geometry)
    : position_{ position }
{
    conflictManager_ = std::make_unique<ConflictManager>(this, geometry);
    controller_ = std::make_unique<UncontrolledIntersection>(this, geometry);
}

void Intersection::addRoad(Road *road)
{
    roads_.emplace_back(road);
}

Connection* Intersection::createConnection(Lane *source, Lane *destination)
{
    // The validation logic that was in Lane::addConnection now belongs here.
    if (!source || !destination)
        throw std::invalid_argument("Cannot create a connection with null lanes.");

    if (this != source->roadway()->destination())
        throw std::invalid_argument("Source lane does not lead into this intersection.");

    if (this != destination->roadway()->source())
        throw std::invalid_argument("Destination lane does not originate from this intersection.");

    // Create and take ownership of the new connection
    auto newConnection = std::make_unique<Connection>(source, destination);

    // Get the raw pointer to return
    Connection* connectionPtr = newConnection.get();

    // Add the connection to the lane for easy access during simulation
    source->addOutgoingConnection(connectionPtr);

    // Store the unique_ptr in this intersection's list
    connections_.emplace_back(std::move(newConnection));

    return connectionPtr;
}

bool Intersection::reachedPriorityRoadwayLimit()
{
    int priorityRoadways{ 0 };
    for (const auto& road : roads_)
    {
        Roadway* approach = road->forwardRoadway()->destination() == this ? road->forwardRoadway() : road->backwardRoadway();
        if (approach->priority() == PriorityType::Priority)
            priorityRoadways++;
        if (priorityRoadways > 2)
            return false;
    }
    return true;
}
