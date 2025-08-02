#pragma once
#include "roadway.h"

class IntersectionMovement
{
    Intersection* parent_;
    Roadway* incomingRoadway_;
    Roadway* outgoingRoadway_;
    float vehicleDemand_;   // vehicles per hour
    float saturationFlowRate_;

public:
    IntersectionMovement(Roadway* incoming, Roadway* outgoing);
    void setDemand(float vehiclesPerHour) { vehicleDemand_ = vehiclesPerHour; }

    const Roadway* getIncoming() const { return incomingRoadway_; }
    const Roadway* getOutgoing() const { return outgoingRoadway_; }
};
