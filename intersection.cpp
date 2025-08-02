#include "intersection.h"
#include <algorithm>   // std::find

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

IntersectionMovement *Intersection::createMovement(Roadway *incoming, Roadway *outgoing)
{
    auto isIncoming = std::find(incomingRoadways_.begin(), incomingRoadways_.end(), incoming);
    if (isIncoming == incomingRoadways_.end())
    {
        return nullptr;
    }

    auto isOutgoing = std::find(outgoingRoadways_.begin(), outgoingRoadways_.end(), outgoing);
    if (isOutgoing == outgoingRoadways_.end())
    {
        return nullptr;
    }

    if (findMovement(incoming, outgoing))
    {
        return nullptr; // Movement already exists.
    }

    movements_.emplace_back(incoming, outgoing);
    return &movements_.back();
}

IntersectionMovement *Intersection::findMovement(Roadway *incoming, Roadway *outgoing)
{
    auto it = std::find_if(movements_.begin(), movements_.end(),
                           [=](const IntersectionMovement& m) {
                               return m.getIncoming() == incoming && m.getOutgoing() == outgoing;
                           });

    if (it != movements_.end())
    {
        return &(*it);
    }
    return nullptr;
}

const IntersectionMovement *Intersection::findMovement(Roadway *incoming, Roadway *outgoing) const
{
    auto it = std::find_if(movements_.begin(), movements_.end(),
                           [=](const IntersectionMovement& m) {
                               return m.getIncoming() == incoming && m.getOutgoing() == outgoing;
                           });

    if (it != movements_.end())
    {
        return &(*it);
    }
    return nullptr;
}





