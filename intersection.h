#pragma once

#include "intersectionapproach.h"
#include <QPointF>
// #include <unordered_map>
// #include <ranges>

class Road;

class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;

public:
    Intersection(QPointF position);
    QPointF getPosition() const { return position_; }
    const std::vector<Road*>& getRoads() const { return roads_; }

    void addRoad(Road *road);

private:
};
