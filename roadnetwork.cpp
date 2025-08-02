#include "roadnetwork.h"
#include "intersection.h"

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
    return roadways_.back().get();
}

