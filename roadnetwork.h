#pragma once

#include "road.h"
#include "geometrymanager.h"
#include "vehicle.h"
#include <QPointF>

class Intersection;

class RoadNetwork
{
    std::vector<std::unique_ptr<Intersection>> intersections_;
    std::vector<std::unique_ptr<Road>> roads_;
    std::unique_ptr<GeometryManager> geometryManager_;
    std::vector<std::unique_ptr<Vehicle>> vehicles_;

public:
    RoadNetwork();
    ~RoadNetwork();
    Intersection* createIntersection(QPointF position);
    Road* createRoad(Intersection* start, Intersection* end);
    const std::vector<std::unique_ptr<Intersection>>& intersections() const { return intersections_; }
    const std::vector<std::unique_ptr<Road>>& roads() const { return roads_; }
    const GeometryManager* geometry() const { return geometryManager_.get(); }
    Vehicle* createVehicle(Lane* initialLane);
    const std::vector<std::unique_ptr<Vehicle>>& vehicles() const { return vehicles_; }
};
