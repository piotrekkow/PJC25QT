#pragma once

#include "lane.h"
#include <memory>
#include <ranges>
#include <QPointF>

class Intersection;
class Road;

class Roadway
{
public:
    enum class RoadwayDirection {
        FORWARD,
        BACKWARD
    };

private:
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;
    std::vector<std::unique_ptr<Lane>> lanes_;  // unique_ptr because of possible vector reallocations making a raw pointer invalid
    RoadwayDirection direction_;
    Road* road_;
    PriorityType priority_;

public: 
    Roadway(Road* parent, Intersection *source, Intersection *destination, RoadwayDirection direction, PriorityType priority);
    Intersection* source() const { return sourceIntersection_; }
    Intersection* destination() const { return destinationIntersection_; }
    PriorityType priority() const { return priority_; }
    void priority(PriorityType priority);

    Lane* addLane();
    auto lanesView() const { return lanes_ | std::views::transform([](auto const& up) { return up.get(); }); }
    const std::vector<std::unique_ptr<Lane>>& lanes() const { return lanes_; }
    bool isForwardRoadway() const { return direction_ == RoadwayDirection::FORWARD; }
    const Road* road() const { return road_; }
};
