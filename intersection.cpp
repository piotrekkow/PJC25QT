#include "intersection.h"

Intersection::Intersection(QPointF position)
    : position_{ position }
{}

void Intersection::addRoad(Road *road)
{
    roads_.emplace_back(road);
}
