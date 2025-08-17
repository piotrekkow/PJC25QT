#pragma once
#include <QLineF>

class Lane;

class IntersectionConnection
{
    Lane* sourceLane_;
    Lane* destinationLane_;
    float sourceOffset_;
    float destinationOffset_;

public:
    IntersectionConnection(Lane* source, Lane* destination);
    Lane* source() const { return sourceLane_; }
    Lane* destination() const { return destinationLane_; }
    float sourceOffset() const { return sourceOffset_; }
    float destinationOffset() const { return destinationOffset_; }

    void sourceOffset(float offset) { sourceOffset_ = offset; }
    void destinationOffset(float offset) { destinationOffset_ = offset; }
};
