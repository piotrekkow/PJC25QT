#pragma once
#include <vector>
#include <optional>

class IntersectionConnection;
class Lane
{
    /// Lane width in meters
    float width_;
    /// Full length lanes (taking up the entire length of a roadway) don't have this value.
    std::optional<float> length_;

    std::vector<IntersectionConnection*> nextConnections_;

public:
    Lane(std::optional<float> length, float width = 3.5f);
    float getWidth() const { return width_; }
};
