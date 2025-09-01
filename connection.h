#pragma once
#include "itraversable.h"

class Lane;

class Connection : public VehicleTraversable
{
    const Lane* sourceLane_;
    const Lane* destinationLane_;
    qreal sourceOffset_;
    qreal destinationOffset_;
    qreal stopLineOffset_;

public:
    Connection(const Lane* source, const Lane* destination);
    const Lane* source() const { return sourceLane_; }
    const Lane* destination() const { return destinationLane_; }

    qreal sourceOffset() const { return sourceOffset_; }
    qreal destinationOffset() const { return destinationOffset_; }
    qreal stopLineOffset() const { return stopLineOffset_; }

    void sourceOffset(qreal offset) { sourceOffset_ = offset; }
    void destinationOffset(qreal offset) { destinationOffset_ = offset; }
    void stopLineOffset(qreal offset) { stopLineOffset_ = offset; }

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    std::vector<const Traversable*> next() const override;
    const Lane* nextLane() const { return destinationLane_; }
    PriorityType regulatoryPriority() const override;
    qreal speedLimit() const override;
    const Intersection* intersection() const override;

    std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const override;
};


// class ConflictManager;

// class Connection : public ITraversable
// {
//     Lane* sourceLane_;
//     Lane* destinationLane_;
//     qreal sourceOffset_;
//     qreal destinationOffset_;
//     qreal stopLineOffset_;

// public:
//     Connection(Lane* source, Lane* destination);
//     Lane* source() const { return sourceLane_; }
//     Lane* destination() const { return destinationLane_; }
//     qreal sourceOffset() const { return sourceOffset_; }
//     qreal destinationOffset() const { return destinationOffset_; }

//     void sourceOffset(qreal offset) { sourceOffset_ = offset; }
//     void destinationOffset(qreal offset) { destinationOffset_ = offset; }

//     // This is a getter, roadwayPriority is set at the roadway level
//     PriorityType roadwayPriority() const override;

//     const Intersection* intersection() const override;

//     qreal stopLineOffset() const { return stopLineOffset_; }

//     /**
//      * @brief checks whether new offset is valid (before first merge or crossing conflict point) and sets it
//      * @param offset distance in meters from the source of connection along connection path
//      * @param conflictManager
//      * @return returns true if new offset applied
//      */
//     bool stopLineOffset(qreal offset, const ConflictManager* conflictManager);

//     const QPainterPath& path(const GeometryManager* geometryManager) const override;
//     qreal length(const GeometryManager* geometryManager) const override;
//     TraversableType type() const override { return TraversableType::Connection; }
//     std::vector<ITraversable*> next() const override;
// };

