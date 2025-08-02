#include "roadnetwork.h"
#include "intersection.h"
#include <QDebug>

RoadNetwork::RoadNetwork() {}

RoadNetwork::~RoadNetwork() = default;

Intersection *RoadNetwork::createIntersection(QPointF position)
{
    intersections_.emplace_back(std::make_unique<Intersection>(position));
    return intersections_.back().get();
}

Roadway *RoadNetwork::createRoadway(Intersection *source, Intersection *destination)
{
    roadways_.emplace_back(std::make_unique<Roadway>(source, destination));
    Roadway* newRoadway{ roadways_.back().get() };

    for (Roadway* candidateRoadway : destination->getOutgoingRoadways())
    {
        if (candidateRoadway->getDestination() == source)
        {
            newRoadway->setOppositeRoadway(candidateRoadway);
            candidateRoadway->setOppositeRoadway(newRoadway);
            break;
        }
    }
    return newRoadway;
}
