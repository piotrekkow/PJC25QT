#pragma once
#include "traversable.h"

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
