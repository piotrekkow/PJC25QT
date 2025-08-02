#include "intersectionmovement.h"

IntersectionMovement::IntersectionMovement(Roadway *incoming, Roadway *outgoing)
    : parent_{ incoming->getDestination() }
    , incomingRoadway_{ incoming }
    , outgoingRoadway_{ outgoing }
    , vehicleDemand_{ 0 }
{}
