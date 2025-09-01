#pragma once
#include "roadway.h"
#include "roadgeometry.h"
#include <utility>

class Intersection;

class Road
{
    const Intersection* startIntersection_;
    const Intersection* endIntersection_;
    std::unique_ptr<Roadway> forwardRoadway_;     // start->end
    std::unique_ptr<Roadway> backwardRoadway_;    // end->start
    std::unique_ptr<RoadGeometry> geometry_;

public:
    Road(const Intersection* start, const Intersection* end, GeometryManager* geometryManager);
    Roadway* createRoadway(Intersection* target);
    std::pair<Roadway*, Roadway*> createRoadways();

    const Intersection* startIntersection() const { return startIntersection_; }
    const Intersection* endIntersection() const { return endIntersection_; }

    const Roadway* roadway(const Intersection* target) const { return (target == startIntersection_) ? backwardRoadway_.get()
                                                                                                     : (target == endIntersection_) ? forwardRoadway_.get()
                                                                                                                                    : nullptr; }

    Roadway* roadway(const Intersection* target) { return (target == startIntersection_) ? backwardRoadway_.get()
                                                          : (target == endIntersection_) ? forwardRoadway_.get()
                                                                                         : nullptr; }

    std::vector<const Roadway*> roadways() const;

    RoadGeometry& geometry() { return *geometry_; }
    const RoadGeometry& geometry() const { return *geometry_; };

    bool has(const Intersection* intersection) const { return intersection == startIntersection_ || intersection == endIntersection_; }
};
