#pragma once
#include "intersection.h"
#include "roadway.h"
#include "roadgeometry.h"
#include <utility>
#include <memory>

class Road
{
    Intersection* primaryIntersection_;
    Intersection* secondaryIntersection_;
    std::unique_ptr<Roadway> primaryRoadway_;   // primary roadway's destination is primary intersection
    std::unique_ptr<Roadway> secondaryRoadway_;
    RoadGeometry geometry_;

public:
    Road(Intersection* primary, Intersection* secondary);
    Roadway* createRoadway(Intersection* target);
    std::pair<Roadway*, Roadway*> createRoadways();

    Intersection* getPrimaryIntersection() const { return primaryIntersection_; }
    Intersection* getSecondaryIntersection() const { return secondaryIntersection_; }
    Roadway* getPrimaryRoadway() const { return primaryRoadway_.get(); }
    Roadway* getSecondaryRoadway() const { return secondaryRoadway_.get(); }

    Roadway* getRoadway(Intersection* target) const;
    RoadGeometry& getGeometry() { return geometry_; }
    const RoadGeometry& getGeometry() const { return geometry_; };
};
