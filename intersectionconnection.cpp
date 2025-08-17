#include "intersectionconnection.h"

IntersectionConnection::IntersectionConnection(Lane *source, Lane *destination)
    : sourceLane_{ source }
    , destinationLane_{ destination }
    , sourceOffset_{ 0.0f }
    , destinationOffset_{ 0.0f }
{
}
