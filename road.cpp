#include "road.h"
// #include "intersection.h"
#include <stdexcept>

Road::Road(const Intersection *primary, const Intersection *secondary, GeometryManager* geometryManager)
    : startIntersection_{ primary }
    , endIntersection_{ secondary }
{
    geometry_ = std::make_unique<RoadGeometry>(this, geometryManager);
}

/**
 * @brief Creates a one-way road. The 'target' is the destination.
 * @param target - destination intersection of the roadway
 * @return
 */
Roadway *Road::createRoadway(Intersection *target)
{
    if (target == endIntersection_)
    {
        forwardRoadway_ = std::make_unique<Roadway>(this, startIntersection_, endIntersection_, PriorityType::Yield);
        return forwardRoadway_.get();
    }
    else if (target == startIntersection_)
    {
        backwardRoadway_ = std::make_unique<Roadway>(this, endIntersection_, startIntersection_, PriorityType::Yield);
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
    forwardRoadway_ = std::make_unique<Roadway>(this, startIntersection_, endIntersection_, PriorityType::Yield);
    backwardRoadway_ = std::make_unique<Roadway>(this, endIntersection_, startIntersection_, PriorityType::Yield);
    return std::pair(forwardRoadway_.get(), backwardRoadway_.get());
}

std::vector<const Roadway *> Road::roadways() const
{
    std::vector<const Roadway*> roadways;

    if (forwardRoadway_) {
        roadways.push_back(forwardRoadway_.get());
    }
    if (backwardRoadway_) {
        roadways.push_back(backwardRoadway_.get());
    }

    return roadways;
}
