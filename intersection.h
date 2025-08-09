#pragma once

#include <QPointF>

class Road;

class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;

public:
    Intersection(QPointF position);
    const std::vector<Road*>& getRoads() const { return roads_; }
    QPointF getPosition() const { return position_; }

    void addRoad(Road *road);

private:
};
