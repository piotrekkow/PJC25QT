#pragma once

#include "intersectionconnection.h"
#include "lane.h"
#include <vector>
#include <QLineF>

class Intersection;

class IntersectionApproach
{
    Intersection* parentIntersection_;
    std::vector<IntersectionConnection> connections_;
    std::vector<Lane*> lanes_;

public:
    IntersectionApproach(Intersection* parent);
    Lane* addLane(float length_);

private:

};
