#pragma once

#include <QPointF>
#include <memory>
#include "unordered_set"
#include "connection.h"
#include "conflictmanager.h"

class Road;

class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;
    std::vector<std::unique_ptr<Connection>> connections_;
    std::unique_ptr<ConflictManager> conflictManager_;

public:
    Intersection(QPointF position, GeometryManager* geometry);

    QPointF position() const { return position_; }
    const std::vector<Road*>& roads() const { return roads_; }
    const std::vector<std::unique_ptr<Connection>>& connections() const { return connections_; }
    ConflictManager* conflictManager() { return conflictManager_.get(); }
    const ConflictManager* conflictManager() const { return conflictManager_.get(); }

    void addRoad(Road *road);
    Connection* createConnection(Lane* source, Lane* destination);
    std::unordered_map<const Roadway*, std::unordered_set<const Roadway*>> roadwayAdjacency() const;
};

