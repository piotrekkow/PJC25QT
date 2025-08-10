#pragma once

#include "intersectionapproach.h"
#include <QPointF>
#include <unordered_map>
#include <ranges>

class Road;

class Intersection
{
    QPointF position_;
    std::unordered_map<Road*, IntersectionApproach> approaches_;

public:
    Intersection(QPointF position);
    auto getRoadsView() const { return std::views::keys(approaches_); }
    QPointF getPosition() const { return position_; }

    void addRoad(Road *road);

private:
};
