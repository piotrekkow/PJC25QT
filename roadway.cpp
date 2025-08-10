#include "roadway.h"

Roadway::Roadway(Road* parent, Intersection *source, Intersection *destination, int laneCount)
    : parent_{ parent }
    , sourceIntersection_{ source }
    , destinationIntersection_{ destination }
    , laneCount_{ laneCount }
{
}


// Lane* Roadway::addLane()
// {
//     lanes_.emplace_back(std::make_unique<Lane>());
//     return lanes_.back().get();
// }
