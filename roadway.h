#pragma once

#include "lane.h"
#include <memory>
// #include <ranges>

class Intersection;
class Road;

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
    const Intersection* source() const { return sourceIntersection_; }
    const Intersection* destination() const { return destinationIntersection_; }
    PriorityType priority() const { return priority_; }
    void priority(PriorityType priority);
    bool isForwardRoadway() const;

    qreal speedLimit() const { return speedLimit_; }
    void speedLimit(qreal value) { speedLimit_ = value; }

    const Lane* addLane();
    // auto lanesView() const { return lanes_ | std::views::transform([](auto const& up) { return up.get(); }); }
    const std::vector<std::unique_ptr<Lane>>& lanes() const { return lanes_; }

    const Road* road() const { return road_; }


private:
    bool canHavePriority();
};
