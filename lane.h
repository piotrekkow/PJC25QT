#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "connection.h"

class Roadway;

class Lane
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
};
