#pragma once

#include <QPointF>

class Road;

class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;

public:
    Intersection(QPointF position);
    QPointF position() const { return position_; }
    const std::vector<Road*>& roads() const { return roads_; }

    void addRoad(Road *road);
};
