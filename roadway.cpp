#include "roadway.h"
#include "intersection.h"

Roadway::Roadway(Intersection *source, Intersection *destination)
    : sourceIntersection_{ source }
    , destinationIntersection_{ destination }
    , oppositeRoadway_{ nullptr }
{
}
