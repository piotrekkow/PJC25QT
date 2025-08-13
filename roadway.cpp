#include "roadway.h"

Roadway::Roadway(Road* parent, Intersection *source, Intersection *destination)
    : parent_{ parent }
    , sourceIntersection_{ source }
    , destinationIntersection_{ destination }
{
    addLane();
}


Lane* Roadway::addLane()
{
    lanes_.emplace_back(std::make_unique<Lane>(std::nullopt));
    return lanes_.back().get();
}

