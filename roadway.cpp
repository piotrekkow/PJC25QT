#include "roadway.h"
#include "intersection.h"

Roadway::Roadway(Road* parent, Intersection *source, Intersection *destination)
    : parent_{ parent }
    , sourceIntersection_{ source }
    , destinationIntersection_{ destination }
{
}

Lane* Roadway::addLane()
{
    lanes_.emplace_back(std::make_unique<Lane>());
    return lanes_.back().get();
}
