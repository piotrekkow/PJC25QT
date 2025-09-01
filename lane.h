#pragma once
#include "itraversable.h"
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




// class Roadway;

// class Lane : public ITraversable
// {
//     /// Lane width in meters
//     qreal width_;
//     /// Full length lanes (taking up the entire length of a roadway) don't have this value.
//     std::optional<qreal> length_;
//     Roadway* roadway_;
//     std::vector<Connection*> connections_;

// public:
//     Lane(Roadway* parent, std::optional<qreal> length = std::nullopt, qreal width = 3.5f);
//     qreal width() const { return width_; }
//     void addOutgoingConnection(Connection* connection);
//     const std::vector<Connection*>& connections() const { return connections_; }
//     const Roadway* roadway() const { return roadway_; }

//     PriorityType roadwayPriority() const override;
//     const QPainterPath& path(const GeometryManager* geometryManager) const override;
//     const Intersection* intersection() const override;

//     qreal length(const GeometryManager* geometryManager) const override;
//     TraversableType type() const override { return TraversableType::Lane; }
//     std::vector<ITraversable*> next() const override;
// };
