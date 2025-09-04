#pragma once

#include "agent.h"
#include "pidcontroller.h"
#include "navigationstrategy.h"
#include <queue>
#include <memory>

class Intersection;
class Connection;
class Lane;
class ConflictData;
class ConflictPoint;

enum class DebugAction
{
    None,
    Proceeding,
    Stopping,
    Following
};

class Vehicle : public Agent
{
    qreal acceleration_;
    qreal cruiseSpeed_;
    qreal nextStopDistance_;

    const qreal maxAcceleration_{ 3.0 };    // m/s^2
    const qreal maxDeceleration_{ 8.0 };    // m/s^2
    const qreal comfAcceleration_{ 1.8 };   // -m/s^2
    const qreal comfDeceleration_{ 2.0 };   // -m/s^2

    const qreal jerk_{ 6.0 };

    const qreal minTimeGap_{ 2.0 };         // s
    const qreal minDistanceGap_{ 1.0 };     // m

    PIDController controller_;

    DebugAction debugAction_{ DebugAction::None };

    std::queue<const Intersection*> routeQueue_;

public:
    static std::unique_ptr<Vehicle> create(Lane* initialLane, const Traffic* traffic, const GeometryManager *geometry);

    qreal nextStopDistance() const { return nextStopDistance_; }
    void nextStopDistance(qreal distance) { nextStopDistance_ = distance; }
    qreal decisionDistance() const;
    Connection* likelyNextConnection() const;   // priv?
    qreal timeToReach(qreal distance);
    qreal acceleration() const { return acceleration_; } // ONLY FOR DEBUG
    qreal cruiseSpeed() const { return cruiseSpeed_; } // ONLY FOR DEBUG
    DebugAction debugAction() const { return debugAction_; } // ONLY FOR DEBUG

protected:
    Vehicle(Lane* initialLane, const Traffic* traffic, const GeometryManager* geometry);

    void applyPhysics(qreal deltaTime) override;
    void updateDynamics(qreal deltaTime) override;
    std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) override;

private:
    bool canSafelyProceed(std::vector<ConflictData> conflicts);
    qreal distanceToConflict(const ConflictPoint* cp) const;
};
