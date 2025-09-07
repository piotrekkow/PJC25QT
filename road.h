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

    /**
     * @brief creates a new roadway to target intersection
     * @return created roadway pointer
     */
    Roadway* createRoadway(Intersection* target);

    /**
     * @brief populates both roadways of a road if they don't yet exist
     * @return pair of created (or already existing) roadway pointers
     */
    std::pair<Roadway*, Roadway*> createRoadways();

    /**
     * @return intersection from which forward roadway originates
     */
    const Intersection* startIntersection() const { return startIntersection_; }

    /**
     * @return intersection from which backward roadway originates
     */
    const Intersection* endIntersection() const { return endIntersection_; }

    /**
     * @return roadway into target intersection
     */
    const Roadway* roadwayInto(const Intersection* target) const { return (target == startIntersection_) ? backwardRoadway_.get()
                                                                                                         : (target == endIntersection_) ? forwardRoadway_.get()
                                                                                                                                        : nullptr; }

    /**
     * @return roadway into target intersection
     */
    Roadway* roadwayInto(const Intersection* target) { return (target == startIntersection_) ? backwardRoadway_.get()
                                                                                             : (target == endIntersection_) ? forwardRoadway_.get()
                                                                                                                            : nullptr; }

    /**
     * @return roadway out of target intersection
     */
    const Roadway* roadwayOutOf(const Intersection* target) const { return (target == startIntersection_) ? forwardRoadway_.get()
                                                                                                          : (target == endIntersection_) ? backwardRoadway_.get()
                                                                                                                                         : nullptr; }

    /**
     * @return roadway out of target intersection
     */
    Roadway* roadwayOutOf(const Intersection* target) { return (target == startIntersection_) ? forwardRoadway_.get()
                                                                                              : (target == endIntersection_) ? backwardRoadway_.get()
                                                                                                                             : nullptr; }

    /**
     * @return roadways belonging to this road
     */
    std::vector<const Roadway*> roadways() const;

    /**
     * @return road geometry
     */
    RoadGeometry& geometry() { return *geometry_; }
    const RoadGeometry& geometry() const { return *geometry_; };

    /**
     * @return information whether this road has intersection at either start or end
     */
    bool has(const Intersection* intersection) const { return intersection == startIntersection_ || intersection == endIntersection_; }
};
