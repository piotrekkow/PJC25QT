#pragma once
#include "itraversable.h"
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

    void update();
    virtual const Traversable* next() const = 0;

protected:
    virtual qreal distanceToStopLine() = 0;
    virtual bool canSafelyProceed(const std::vector<ConflictData>& conflicts) const = 0;
    virtual const std::vector<ConflictData> conflictsMustYieldTo() const = 0;

private:
    qreal timeToReach(qreal distance) const;
};

// Lane-specific navigation
class LaneNavigationStrategy : public NavigationStrategy {
    const Lane* lane_;
    const Roadway* designatedNextRoadway_;

public:
    explicit LaneNavigationStrategy(Vehicle* vehicle, const Traffic* traffic, const GeometryManager* geometry, const Lane* lane);

    const Traversable* next() const override;

protected:
    qreal distanceToStopLine() override;
    bool canSafelyProceed(const std::vector<ConflictData>& conflicts) const override;
    const std::vector<ConflictData> conflictsMustYieldTo() const override;

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
    bool canSafelyProceed(const std::vector<ConflictData>& conflicts) const override;
    const std::vector<ConflictData> conflictsMustYieldTo() const override;

private:
    qreal distanceToConflict(const ConflictPoint* cp) const;
};
