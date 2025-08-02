#pragma once

#include "roadway.h"
#include <vector>
#include <memory>
#include <QPointF>

class Intersection;

class RoadNetwork
{
    std::vector<std::unique_ptr<Intersection>> intersections_;
    std::vector<std::unique_ptr<Roadway>> roadways_;

public:
    RoadNetwork();
    ~RoadNetwork();
    Intersection* createIntersection(QPointF position);
    Roadway* createRoadway(Intersection* source, Intersection* destination);
};
