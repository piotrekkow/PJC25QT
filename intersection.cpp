#include "intersection.h"

Intersection::Intersection(QPointF position)
    : position_{ position }
{}

void Intersection::addIncomingRoadway(Roadway *roadway)
{
    incomingRoadways_.push_back(roadway);
}

void Intersection::addOutgoingRoadway(Roadway *roadway)
{
    outgoingRoadways_.push_back(roadway);
}

