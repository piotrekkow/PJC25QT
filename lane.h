#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "intersectionconnection.h"

class Roadway;

class Lane
{
    /// Lane width in meters
    float width_;
    /// Full length lanes (taking up the entire length of a roadway) don't have this value.
    std::optional<float> length_;
    Roadway* roadway_;
    std::vector<std::unique_ptr<IntersectionConnection>> connections_;

public:
    Lane(Roadway* parent, std::optional<float> length = std::nullopt, float width = 3.5f);
    float width() const { return width_; }
    IntersectionConnection* addConnection(Lane* target);
    const std::vector<std::unique_ptr<IntersectionConnection>>& connections() const { return connections_; }

private:
    size_t index() const;
};
