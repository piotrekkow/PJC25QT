#pragma once
#include "traversable.h"
#include "connection.h"

class Roadway;

/**
 * @brief Lanes are traversable objects along which vehicles travel between intersections. They're a part of a roadway.
 */
class Lane : public VehicleTraversable
{
    std::vector<const Connection*> connections_;
    qreal width_;
    const Roadway* roadway_;

public:
    Lane(const Roadway* roadway, qreal width = 3.5)
        : width_{ width }
        , roadway_{ roadway }
    {};

    /**
     * @brief adds connection to lane, this connection has to already have been created by intersection and originate at this lane.
     */
    void addConnection(const Connection* connection);

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    std::vector<const Traversable*> next() const override;
    PriorityType regulatoryPriority() const override;
    qreal speedLimit() const override;
    const Intersection* intersection() const override;

    /**
     * @return width in m
     */
    qreal width() const { return width_; }

    /**
     * @return parent roadway of lane
     */
    const Roadway* roadway() const { return roadway_; }

    /**
     * @return vector of all connections originating from this lane
     */
    std::vector<const Connection*> nextConnections() const { return connections_; };

    std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const override;
};
