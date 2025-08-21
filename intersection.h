#pragma once

#include <QPointF>
#include <memory>
#include "IntersectionControlType.h"
#include "connection.h"
#include "conflictmanager.h"
#include "iintersectioncontroller.h"

class Road;

class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;
    std::vector<std::unique_ptr<Connection>> connections_;
    // std::unordered_map<Roadway*, PriorityRule> roadwayPriorities_;

    IntersectionControlType controlType_;
    std::unique_ptr<IIntersectionController> controller_;
    std::unique_ptr<ConflictManager> conflictManager_;


public:
    Intersection(QPointF position);
    QPointF position() const { return position_; }
    const std::vector<Road*>& roads() const { return roads_; }
    const std::vector<std::unique_ptr<Connection>>& connections() const { return connections_; }
    ConflictManager* conflictManager() { return conflictManager_.get(); }
    const ConflictManager* conflictManager() const { return conflictManager_.get(); }

    void addRoad(Road *road);
    Connection* createConnection(Lane* source, Lane* destination);
};
