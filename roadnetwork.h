#pragma once

#include "road.h"
#include <QPointF>

class Intersection;

class RoadNetwork
{
    std::vector<std::unique_ptr<Intersection>> intersections_;
    std::vector<std::unique_ptr<Road>> roads_;

public:
    RoadNetwork();
    ~RoadNetwork();
    Intersection* createIntersection(QPointF position);
    Road* createRoad(Intersection* primary, Intersection* secondary);
    const std::vector<std::unique_ptr<Intersection>>& getIntersections() const { return intersections_; }
    const std::vector<std::unique_ptr<Road>>& getRoads() const { return roads_; }
};
