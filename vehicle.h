#pragma once

#include "agent.h"
#include "drivermodel.h"
#include <memory>

class Intersection;
class Connection;
class Lane;
class ConflictData;
class ConflictPoint;

struct DecisionContext
{
    const Vehicle* leadVehicle = nullptr;
    qreal distanceToStop = std::numeric_limits<qreal>::max();

    qreal speedLimit;
    qreal acceleration;

    const qreal maxAcceleration;
    const qreal maxDeceleration;
    const qreal maxJerk;
};

class Vehicle : public Agent
{
    qreal acceleration_;
    qreal cruiseSpeed_; // TODO: change speed limit since DriverModel will have desiredSpeed

    const qreal maxAcceleration_{ 3.0 };    // m/s^2
    const qreal maxDeceleration_{ 8.0 };    // m/s^2
    const qreal maxJerk_{ 6.0 };            // m/s^3

    std::unique_ptr<DriverModel> driver_;

public:
    static std::unique_ptr<Vehicle> create(Lane* initialLane, const Traffic* traffic, const GeometryManager *geometry);

    qreal timeToReach(qreal distance);
    qreal acceleration() const { return acceleration_; } // ONLY FOR DEBUG
    qreal cruiseSpeed() const { return cruiseSpeed_; } // ONLY FOR DEBUG
    DriverAction debugAction() const { return driver_->action(); } // ONLY FOR DEBUG

protected:
    Vehicle(Lane* initialLane, const Traffic* traffic, const GeometryManager* geometry);

    void applyPhysics(qreal deltaTime) override;
    void updateDynamics(qreal deltaTime) override;
    std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) override;

private:
    bool canSafelyProceed(std::vector<ConflictData> conflicts);
    qreal distanceToConflict(const ConflictPoint* cp) const;
    void applyAccelerationLimits(qreal desiredAcceleration, qreal deltaTime);

    const Vehicle* getLeadVehicle() const;

    DecisionContext decisionContext() const;
};
