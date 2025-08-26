#pragma once
#include "itraversable.h"


class Lane;
class ConflictManager;

class Connection : public ITraversable
{
    Lane* sourceLane_;
    Lane* destinationLane_;
    qreal sourceOffset_;
    qreal destinationOffset_;
    qreal stopLineOffset_;

public:
    Connection(Lane* source, Lane* destination);
    Lane* source() const { return sourceLane_; }
    Lane* destination() const { return destinationLane_; }
    qreal sourceOffset() const { return sourceOffset_; }
    qreal destinationOffset() const { return destinationOffset_; }

    void sourceOffset(qreal offset) { sourceOffset_ = offset; }
    void destinationOffset(qreal offset) { destinationOffset_ = offset; }

    // This is a getter, roadwayPriority is set at the roadway level
    PriorityType roadwayPriority() const override;

    const Intersection* intersection() const override;

    qreal stopLineOffset() const { return stopLineOffset_; }

    /**
     * @brief checks whether new offset is valid (before first merge or crossing conflict point) and sets it
     * @param offset distance in meters from the source of connection along connection path
     * @param conflictManager
     * @return returns true if new offset applied
     */
    bool stopLineOffset(qreal offset, const ConflictManager* conflictManager);

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    TraversableType type() const override { return TraversableType::Connection; }
    std::vector<ITraversable*> next() const override;
};
