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

    /**
     * @return lane from which the connection starts
     */
    const Lane* source() const { return sourceLane_; }

    /**
     * @return lane at which the connection ends
     */
    const Lane* destination() const { return destinationLane_; }

    /**
     * @return offset in a straight line along the source lane's normal in m
     */
    qreal sourceOffset() const { return sourceOffset_; }

    /**
     * @return offset in a straight line along the destination lane's normal in m
     */
    qreal destinationOffset() const { return destinationOffset_; }

    /**
     * @return offset along connection where agents will stop
     */
    qreal stopLineOffset() const { return stopLineOffset_; }

    void sourceOffset(qreal offset) { sourceOffset_ = offset; }
    void destinationOffset(qreal offset) { destinationOffset_ = offset; }

    // TODO: Queueing behind vehicle at different traversable, until then stopLineOffset will break logic for queueing vehicles
    void stopLineOffset(qreal offset) { stopLineOffset_ = offset; }

    const QPainterPath& path(const GeometryManager* geometryManager) const override;
    qreal length(const GeometryManager* geometryManager) const override;
    std::vector<const Traversable*> next() const override;
    PriorityType regulatoryPriority() const override;
    const Intersection* intersection() const override;
    qreal speedLimit() const override;

    std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const override;
};
