#pragma once
#include "traversable.h"
#include <qtypes.h>
#include <vector>

class ConflictData;
class Vehicle;
class Traffic;
class GeometryManager;
class Roadway;
class IIntersectionController;
class Lane;
class Connection;
class ConflictPoint;
class VehicleTraversable;

/**
 * @brief Provides strategy for navigating a traversable.
 */
class NavigationStrategy
{
protected:
    Vehicle* vehicle_;
    const Traffic* traffic_;
    const GeometryManager* geometry_;


    const qreal minTimeGap_{ 2.0 }; // s
    const qreal minDistanceGap_{ 1.5 }; // m

public:
    explicit NavigationStrategy(Vehicle* vehicle, const Traffic* traffic, const GeometryManager* geometry)  // TODO: validate designatedDirectionOfTravel
        : vehicle_{ vehicle }, traffic_{ traffic }, geometry_{ geometry } {}

    virtual ~NavigationStrategy() = default;
    virtual const Traversable* next() const = 0;
    /**
     * @return distance to stop from start of traversable or a very large value if there are no objects agents on this trajectory have to give way to
     */
    qreal distanceToStop();

protected:
    /**
     * @return distance to stop line from the start of traversable
     */
    virtual qreal distanceToStopLine() = 0;

    /**
     * @brief canSafelyProceed
     * @param vector of conflict data provided by conflictsMustYieldTo
     * @return
     */
    virtual bool canSafelyProceed(const std::vector<ConflictData>& conflicts) = 0;

    /**
     * @brief chooses the right traversable and calculates conflicts which have to be resolved
     * @return vector of conflict data calculated by FlowController
     */
    virtual const std::vector<ConflictData> conflictsMustYieldTo() const = 0;

    /**
     * @return distance to stop line based on current traversable from the start of traversable
     */
    virtual qreal calculateDistanceToStopLine() const = 0;
};

// Lane-specific navigation
class LaneNavigationStrategy : public NavigationStrategy {
    const Lane* lane_;
    const Roadway* designatedNextRoadway_;
    bool hasStopObligation_;

public:
    explicit LaneNavigationStrategy(Vehicle* vehicle, const Traffic* traffic, const GeometryManager* geometry, const Lane* lane);

    const Traversable* next() const override;

protected:
    qreal distanceToStopLine() override;
    bool canSafelyProceed(const std::vector<ConflictData>& conflicts) override;
    const std::vector<ConflictData> conflictsMustYieldTo() const override;
    qreal calculateDistanceToStopLine() const override;

private:
    const Connection* nextConnection() const;
    qreal distanceToConflict(const ConflictPoint* cp) const;

};

// Connection-specific navigation
class ConnectionNavigationStrategy : public NavigationStrategy {
    const Connection* connection_;
public:
    explicit ConnectionNavigationStrategy(Vehicle* vehicle, const Traffic* traffic, const GeometryManager* geometry, const Connection* connection)
        : NavigationStrategy(vehicle, traffic, geometry), connection_{ connection } {}

    const Traversable* next() const override;

protected:
    qreal distanceToStopLine() override;
    bool canSafelyProceed(const std::vector<ConflictData>& conflicts) override;
    const std::vector<ConflictData> conflictsMustYieldTo() const override;
    qreal calculateDistanceToStopLine() const override;

private:
    qreal distanceToConflict(const ConflictPoint* cp) const;
};
