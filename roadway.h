#pragma once

class Intersection;

class Roadway
{
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;
    Roadway* oppositeRoadway_;

public:
    Roadway(Intersection* source, Intersection* destination);
    Intersection* getSource() const { return sourceIntersection_; }
    Intersection* getDestination() const { return destinationIntersection_; }
    Roadway* getOppositeRoadway() { return oppositeRoadway_; }    // Used in Renderer to simplify offseting logic
    void setOppositeRoadway(Roadway* roadway) { oppositeRoadway_ = roadway; }
};
