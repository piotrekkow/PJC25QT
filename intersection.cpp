#include "intersection.h"

Intersection::Intersection(QPointF position)
    : position_{ position }
{}

void Intersection::addRoad(Road *road)
{
    approaches_.emplace(road, IntersectionApproach{});
}
