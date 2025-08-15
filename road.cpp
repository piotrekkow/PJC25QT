#include "road.h"
#include <stdexcept>

Road::Road(Intersection *primary, Intersection *secondary)
    : startIntersection_{ primary }
    , endIntersection_{ secondary }
    , geometry_(this)
{
}

Roadway *Road::createRoadway(Intersection *target)
{
    if (target == startIntersection_)
    {
        Intersection* source = endIntersection_;
        forwardRoadway_ = std::make_unique<Roadway>(source, target, RoadwayDirection::FORWARD);
        return forwardRoadway_.get();
    }
    else if (target == endIntersection_)
    {
        Intersection* source = startIntersection_;
        backwardRoadway_ = std::make_unique<Roadway>(source, target, RoadwayDirection::BACKWARD);
        return backwardRoadway_.get();
    }
    else
        throw std::invalid_argument("Provided target intersection not at either endpoint of this road");
}

std::pair<Roadway *, Roadway *> Road::createRoadways()
{
    forwardRoadway_ = std::make_unique<Roadway>(endIntersection_, startIntersection_, RoadwayDirection::FORWARD);
    backwardRoadway_ = std::make_unique<Roadway>(startIntersection_, endIntersection_, RoadwayDirection::BACKWARD);
    return std::pair(forwardRoadway_.get(), backwardRoadway_.get());
}

Roadway *Road::getRoadway(Intersection *target) const
{
    if (target == startIntersection_)
    {
        return forwardRoadway_.get();
    }
    else if (target == endIntersection_)
    {
        return backwardRoadway_.get();
    }
    else
        throw std::invalid_argument("Provided target intersection not at either endpoint of this road");
}
