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

Road *RoadNetwork::createRoad(Intersection *startIntersection, Intersection *endIntersection)
{
    roads_.emplace_back(std::make_unique<Road>(startIntersection, endIntersection));
    Road* newRoad{ roads_.back().get() };

    startIntersection->addRoad(newRoad);
    endIntersection->addRoad(newRoad);

    return newRoad;
}
