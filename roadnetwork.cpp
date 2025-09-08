#include "roadnetwork.h"
#include "intersection.h"
#include <QDebug>

RoadNetwork::RoadNetwork()
{
    geometryManager_ = std::make_unique<GeometryManager>(this);
}

RoadNetwork::~RoadNetwork() = default;

Intersection *RoadNetwork::createIntersection(QPointF position)
{
    intersections_.emplace_back(std::make_unique<Intersection>(position, geometryManager_.get()));
    return intersections_.back().get();
}

Road *RoadNetwork::createRoad(Intersection *startIntersection, Intersection *endIntersection, bool withBothRoadways)
{
    roads_.emplace_back(std::make_unique<Road>(startIntersection, endIntersection, geometryManager_.get()));
    Road* newRoad{ roads_.back().get() };

    startIntersection->addRoad(newRoad);
    endIntersection->addRoad(newRoad);

    if (withBothRoadways)
        newRoad->createRoadways();

    return newRoad;
}
