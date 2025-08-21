#pragma once
#include <QLineF>

class Lane;

class Connection
{
    Lane* sourceLane_;
    Lane* destinationLane_;
    qreal sourceOffset_;
    qreal destinationOffset_;

public:
    Connection(Lane* source, Lane* destination);
    Lane* source() const { return sourceLane_; }
    Lane* destination() const { return destinationLane_; }
    qreal sourceOffset() const { return sourceOffset_; }
    qreal destinationOffset() const { return destinationOffset_; }

    void sourceOffset(qreal offset) { sourceOffset_ = offset; }
    void destinationOffset(qreal offset) { destinationOffset_ = offset; }
};
