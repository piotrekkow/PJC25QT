#pragma once

#include "lane.h"
#include <memory>
// #include <ranges>

class Intersection;
class Road;

/**
 * @brief Roadway manages all lanes in a given direction
 */
class Roadway
{
private:
    const Intersection* sourceIntersection_;
    const Intersection* destinationIntersection_;
    std::vector<std::unique_ptr<Lane>> lanes_;  // unique_ptr because of possible vector reallocations making a raw pointer invalid
    const Road* road_;
    PriorityType priority_;
    qreal speedLimit_;

public: 
    Roadway(const Road* parent, const Intersection *source, const Intersection *destination, PriorityType priority);

    /**
     * @return intersection from which the roadway originates
     */
    const Intersection* source() const { return sourceIntersection_; }

    /**
     * @return intersection at which the roadway ends
     */
    const Intersection* destination() const { return destinationIntersection_; }

    /**
     * @return regulatory priority of roadway (stop, yield, priority)
     */
    PriorityType priority() const { return priority_; }
    void priority(PriorityType priority);

    /**
     * @return is roadway facing forward (the same direction as parent road)
     */
    bool isForwardRoadway() const;

    /**
     * @return speed limit in m/s
     */
    qreal speedLimit() const { return speedLimit_; }
    void speedLimit(qreal value) { speedLimit_ = value; }

    /**
     * @brief creates a new lane to the right of existing lanes
     * @return pointer to newly created lane
     */
    const Lane* addLane();

    /**
     * @return reference to vector of all lanes in the roadway
     */
    const std::vector<std::unique_ptr<Lane>>& lanes() const { return lanes_; }

    /**
     * @return parent road of this roadway
     */
    const Road* road() const { return road_; }


private:
    bool canHavePriority();
};
