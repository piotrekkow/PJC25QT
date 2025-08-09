#include "road.h"
#include <stdexcept>

Road::Road(Intersection *primary, Intersection *secondary)
    : primaryIntersection_{ primary }
    , secondaryIntersection_{ secondary }
{
}

Roadway *Road::createRoadway(Intersection *target)
{
    if (target == primaryIntersection_)
    {
        Intersection* source = secondaryIntersection_;
        primaryRoadway_ = std::make_unique<Roadway>(this, source, target);
        return primaryRoadway_.get();
    }
    else if (target == secondaryIntersection_)
    {
        Intersection* source = primaryIntersection_;
        secondaryRoadway_ = std::make_unique<Roadway>(this, source, target);
        return secondaryRoadway_.get();
    }
    else
        throw std::invalid_argument("Provided target intersection not at either endpoint of this road");
}

std::pair<Roadway *, Roadway *> Road::createRoadways()
{
    primaryRoadway_ = std::make_unique<Roadway>(this, secondaryIntersection_, primaryIntersection_);
    secondaryRoadway_ = std::make_unique<Roadway>(this, primaryIntersection_, secondaryIntersection_);
    return std::pair(primaryRoadway_.get(), secondaryRoadway_.get());
}
