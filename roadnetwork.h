#pragma once

#include "road.h"
#include "geometrymanager.h"

class Intersection;

/**
 * @brief Contains static topology of a road network
 */
class RoadNetwork
{
    std::vector<std::unique_ptr<Intersection>> intersections_;
    std::vector<std::unique_ptr<Road>> roads_;
    std::unique_ptr<GeometryManager> geometryManager_;

public:
    RoadNetwork();
    ~RoadNetwork();

    /**
     * @brief creates an intersection
     * @param position in {m, m}
     * @return pointer to created intersection
     */
    Intersection* createIntersection(QPointF position);

    /**
     * @brief creates a road connecting 2 intersections
     * @param start 1st intersection
     * @param end 2nd intersection
     * @return pointer to created road
     */
    Road* createRoad(Intersection* start, Intersection* end);

    /**
     * @return reference to vector of all intersections in network
     */
    const std::vector<std::unique_ptr<Intersection>>& intersections() const { return intersections_; }

    /**
     * @return reference to vector of all roads in network
     */
    const std::vector<std::unique_ptr<Road>>& roads() const { return roads_; }

    /**
     * @return class managing geometry of topology defined by roads and intersections
     */
    const GeometryManager* geometry() const { return geometryManager_.get(); }
    GeometryManager* geometry() { return geometryManager_.get(); }
};
