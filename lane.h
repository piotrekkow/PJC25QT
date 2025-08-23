#pragma once
#include "itraversable.h"
#include <optional>
#include "connection.h"

class Roadway;

class Lane : public ITraversable
{
    /// Lane width in meters
    qreal width_;
    /// Full length lanes (taking up the entire length of a roadway) don't have this value.
    std::optional<qreal> length_;
    Roadway* roadway_;
    std::vector<Connection*> connections_;

public:
    Lane(Roadway* parent, std::optional<qreal> length = std::nullopt, qreal width = 3.5f);
    qreal width() const { return width_; }
    void addOutgoingConnection(Connection* connection);
    const std::vector<Connection*>& connections() const { return connections_; }
    const Roadway* roadway() const { return roadway_; }

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    TraversableType type() const override { return TraversableType::Lane; }
    std::vector<ITraversable*> next() const override;
};
