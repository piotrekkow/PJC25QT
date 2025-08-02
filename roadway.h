#pragma once

class Intersection;

class Roadway
{
    Intersection* sourceIntersection_;
    Intersection* destinationIntersection_;

public:
    Roadway(Intersection* source, Intersection* destination);
};
