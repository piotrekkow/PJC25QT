#pragma once
#include "traversable.h"
#include "connection.h"

class Roadway;

class Lane : public VehicleTraversable
{
    std::vector<const Connection*> connections_;
    qreal width_;
    const Roadway* roadway_;

public:
    Lane(const Roadway* roadway, qreal width = 3.5) : width_{ width }, roadway_{ roadway } {};
    void addConnection(const Connection* connection);

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    std::vector<const Traversable*> next() const override;
    PriorityType regulatoryPriority() const override;
    qreal speedLimit() const override;
    const Intersection* intersection() const override;

    qreal width() const { return width_; }
    const Roadway* roadway() const { return roadway_; }
    std::vector<const Connection*> nextConnections() const { return connections_; };

    std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const override;
};
