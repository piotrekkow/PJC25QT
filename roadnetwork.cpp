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

Road *RoadNetwork::createRoad(Intersection *primary, Intersection *secondary)
{
    roads_.emplace_back(std::make_unique<Road>(primary, secondary));
    Road* newRoad{ roads_.back().get() };

    primary->addRoad(newRoad);
    secondary->addRoad(newRoad);

    return newRoad;
}
