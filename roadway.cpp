#include "roadway.h"
#include "road.h"

Roadway::Roadway(Intersection *source, Intersection *destination, RoadwayDirection direction)
    : sourceIntersection_{ source }
    , destinationIntersection_{ destination }
    , direction_{ direction }
{
    addLane();
}


Lane* Roadway::addLane()
{
    lanes_.emplace_back(std::make_unique<Lane>(this));
    return lanes_.back().get();
}

