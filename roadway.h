#pragma once

class Intersection;
class Road;

class Roadway
{
    Road* parent_;
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;
    int laneCount_;

public:
    Roadway(Road* parent, Intersection *source, Intersection *destination, int laneCount = 1);
    Intersection* getSource() const { return sourceIntersection_; }
    Intersection* getDestination() const { return destinationIntersection_; }
    void setLaneCount(int numberOfLanes) { laneCount_ = numberOfLanes; }
};
