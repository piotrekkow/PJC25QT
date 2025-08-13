#pragma once

#include "lane.h"
#include <memory>
#include <ranges>

class Intersection;
class Road;

class Roadway
{
    Road* parent_;
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;
    std::vector<std::unique_ptr<Lane>> lanes_;  // unique_ptr because of possible vector reallocations making a raw pointer invalid

public:
    Roadway(Road* parent, Intersection *source, Intersection *destination);
    Intersection* getSource() const { return sourceIntersection_; }
    Intersection* getDestination() const { return destinationIntersection_; }
    Lane* addLane();
    auto getLanesView() const { return lanes_ | std::views::transform([](auto const& up) { return up.get(); }); }
};
