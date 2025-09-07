#include "intersection.h"
#include "road.h"

Intersection::Intersection(QPointF position, GeometryManager* geometry)
    : position_{ position }
{
    conflictManager_ = std::make_unique<ConflictManager>(this, geometry);
}

void Intersection::addRoad(Road *road)
{
    if (!road->has(this))
        throw std::invalid_argument("Tried to add road to intersection which doesn't lead or originate at intersection.");
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

Connection *Intersection::createConnection(Road *source, Road *destination)
{
    auto incomingRoadway = source->roadwayInto(this);
    auto outgoingRoadway = destination->roadwayOutOf(this);

    if (!incomingRoadway || !outgoingRoadway)
        throw std::invalid_argument("Invalid roads for connection creation.");

    if (incomingRoadway->lanes().size() != 1 || outgoingRoadway->lanes().size() != 1)
        throw std::invalid_argument("Cannot create a connection between roads. Multi-lane roadways. Use createConnection(Lane *source, Lane *destination) instead.");

    return createConnection(incomingRoadway->lanes()[0].get(), outgoingRoadway->lanes()[0].get());
}

std::unordered_map<const Roadway*, std::unordered_set<const Roadway*>> Intersection::roadwayAdjacency() const
{
    std::unordered_map<const Roadway*, std::unordered_set<const Roadway*>> adj;

    for (auto& c : connections_)
    {
        const Roadway* from = c->source()->roadway();
        const Roadway* to   = c->destination()->roadway();

        adj[from].insert(to);
    }

    return adj;
}
