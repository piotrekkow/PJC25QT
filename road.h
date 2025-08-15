#pragma once
#include "intersection.h"
#include "roadway.h"
#include "roadgeometry.h"
#include <utility>
#include <memory>

class Road
{
    Intersection* startIntersection_;
    Intersection* endIntersection_;
    std::unique_ptr<Roadway> forwardRoadway_;   // primary roadway's destination is primary intersection
    std::unique_ptr<Roadway> backwardRoadway_;
    RoadGeometry geometry_;

public:
    Road(Intersection* start, Intersection* end);
    Roadway* createRoadway(Intersection* target);
    std::pair<Roadway*, Roadway*> createRoadways();

    Intersection* startIntersection() const { return startIntersection_; }
    Intersection* endIntersection() const { return endIntersection_; }
    Roadway* forwardRoadway() const { return forwardRoadway_.get(); }
    Roadway* backwardRoadway() const { return backwardRoadway_.get(); }

    Roadway* getRoadway(Intersection* target) const;
    RoadGeometry& geometry() { return geometry_; }
    const RoadGeometry& geometry() const { return geometry_; };
    auto roadways() const {
        std::array<const Roadway*, 2> ptrs = { forwardRoadway_.get(), backwardRoadway_.get() };

        return ptrs | std::views::filter([](const Roadway* r) {
                   return r != nullptr;
               });
    }
};
