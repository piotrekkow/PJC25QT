#include "road.h"
#include <stdexcept>

Road::Road(Intersection *primary, Intersection *secondary, GeometryManager* geometryManager)
    : startIntersection_{ primary }
    , endIntersection_{ secondary }
{
    geometry_ = std::make_unique<RoadGeometry>(this, geometryManager);
}

//


/**
 * @brief Creates a one-way road. The 'target' is the destination.
 * @param target - destination intersection of the roadway
 * @return
 */
Roadway *Road::createRoadway(Intersection *target)
{
    if (target == endIntersection_)
    {
        forwardRoadway_ = std::make_unique<Roadway>(this, startIntersection_, endIntersection_, Roadway::RoadwayDirection::FORWARD, PriorityType::Yield);
        return forwardRoadway_.get();
    }
    else if (target == startIntersection_)
    {
        backwardRoadway_ = std::make_unique<Roadway>(this, endIntersection_, startIntersection_, Roadway::RoadwayDirection::BACKWARD, PriorityType::Yield);
        return backwardRoadway_.get();
    }
    else
    {
        throw std::invalid_argument("Provided target intersection not at either endpoint of this road");
    }
}
/**
 * @brief Creates a two-way road.
 * @return
 */
std::pair<Roadway *, Roadway *> Road::createRoadways()
{
    forwardRoadway_ = std::make_unique<Roadway>(this, startIntersection_, endIntersection_, Roadway::RoadwayDirection::FORWARD, PriorityType::Yield);
    backwardRoadway_ = std::make_unique<Roadway>(this, endIntersection_, startIntersection_, Roadway::RoadwayDirection::BACKWARD, PriorityType::Yield);
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

std::vector<const Roadway *> Road::roadways() const
{
    std::vector<const Roadway*> result;
    // Pre-allocating memory is a good practice for performance
    result.reserve(2);

    if (forwardRoadway_) {
        result.push_back(forwardRoadway_.get());
    }
    if (backwardRoadway_) {
        result.push_back(backwardRoadway_.get());
    }

    // This vector now owns the pointers and is safely
    // returned by value.
    return result;
}
