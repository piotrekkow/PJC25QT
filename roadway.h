#pragma once
#include "lane.h"
#include <vector>
#include <memory>

class Intersection;
class Road;

class Roadway
{
    Road* parent_;
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;
    std::vector<std::unique_ptr<Lane>> lanes_;

public:
    Roadway(Road* parent, Intersection *source, Intersection *destination);
    Intersection* getSource() const { return sourceIntersection_; }
    Intersection* getDestination() const { return destinationIntersection_; }
    Lane* addLane();
};
