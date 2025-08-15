#include "intersectionconnection.h"

IntersectionConnection::IntersectionConnection(Lane *source, Lane *destination)
    : sourceLane_{ source }
    , destinationLane_{ destination }
{
}
