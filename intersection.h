#pragma once

#include <QPointF>
#include <memory>
#include "unordered_set"
#include "connection.h"
#include "conflictmanager.h"

class Road;

/**
 * @brief The most basic building block of a road network.
 */
class Intersection
{
    QPointF position_;
    std::vector<Road*> roads_;
    std::vector<std::unique_ptr<Connection>> connections_;
    std::unique_ptr<ConflictManager> conflictManager_;

public:
    Intersection(QPointF position, GeometryManager* geometry);

    /**
     * @return position {m, m}
     */
    QPointF position() const { return position_; }

    /**
     * @return roads connected to intersection
     */
    const std::vector<Road*>& roads() const { return roads_; }

    /**
     * @return connections owned by intersection
     */
    const std::vector<std::unique_ptr<Connection>>& connections() const { return connections_; }

    /**
     * @return conflict manager of intersection
     */
    ConflictManager* conflictManager() { return conflictManager_.get(); }
    const ConflictManager* conflictManager() const { return conflictManager_.get(); }

    /**
     * @brief adds road to intersection, this road has to already have been created and lead to or originate at intersection.
     */
    void addRoad(Road *road);

    /**
     * @brief creates connection at intersection
     * @param source origin of connection
     * @param destination end of connection
     * @return created connection
     */
    Connection* createConnection(Lane* source, Lane* destination);

    /**
     * @brief creates connection at intersection
     * @param source origin of connection, has to have one incoming lane into intersection
     * @param destination end of connection, has to have one outgoin lane out of intersection
     * @return created connection
     */
    Connection* createConnection(Road *source, Road *destination);

    /**
     * @return map of outgoing roadways connected to an incoming roadway.
     */
    std::unordered_map<const Roadway*, std::unordered_set<const Roadway*>> roadwayAdjacency() const;
};

