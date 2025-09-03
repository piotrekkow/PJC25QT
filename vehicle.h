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

class Vehicle : public Agent
{
    qreal acceleration_;
    qreal cruiseSpeed_;
    qreal nextStopDistance_;

    const qreal maxAcceleration_{ 3.0 };    // m/s^2
    const qreal maxDeceleration_{ 8.0 };    // m/s^2
    const qreal comfAcceleration_{ 1.8 };   // -m/s^2
    const qreal comfDeceleration_{ 2.0 };   // -m/s^2

    const qreal minTimeGap_{ 2.0 };         // m
    const qreal minDistanceGap_{ 1.0 };     // s

    PIDController stoppingController_;
    PIDController followingController_;
    PIDController cruiseController_;

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

protected:
    Vehicle(Lane* initialLane, const Traffic* traffic, const GeometryManager* geometry);

    void applyPhysics(qreal deltaTime) override;
    void updateDynamics(qreal deltaTime) override;
    std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) override;

private:
    bool canSafelyProceed(std::vector<ConflictData> conflicts);
    qreal distanceToConflict(const ConflictPoint* cp) const;

};

/*
#include "ConflictData.h"
#include "vehiclepid.h"
#include <QPointF>
#include <QColor>

class Lane;
class GeometryManager;
class ITraversable;
class Traffic;
class VehiclePID;
struct ConflictData;

class Vehicle
{
    const GeometryManager* geometryManager_;
    ITraversable* currentTraversable_;
    qreal progress_;
    bool hasReachedDestination_;

    qreal currentSpeed_; // in m/s
    qreal cruiseSpeed_;  // in m/s
    qreal currentAcceleration_; // in m/s^2

    qreal angle_;      // in degrees
    QPointF position_;

    QColor color_;
    qreal length_;
    qreal width_;

    qreal maxAcceleration_;          // m/s^2, e.g., 3.0
    qreal comfortableAcceleration_;
    qreal comfortableDeceleration_;  // m/s^2, e.g., 2.5 (used for normal yielding)
    qreal maxDeceleration_;          // m/s^2, e.g., 8.0 (used for emergency stops)

    const Traffic* traffic_;
    VehiclePID pidController_;
    qreal safetyTimeGap_{ 2.0 };
    qreal safetyMinDistance_{ 1.5 };
    qreal nextStopDistance_;

public:
    Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, Traffic* traffic, qreal initialPosition = 0.0);
    void update(qreal deltaTime);

    QPointF position() const { return position_; }
    qreal angle() const { return angle_; }
    QColor color() const { return color_; }
    qreal length() const { return length_; }
    qreal width() const { return width_; }
    qreal progress() const { return progress_; }
    qreal speed() const { return currentSpeed_; }
    qreal acceleration() const { return currentAcceleration_; }
    qreal decisionDistance() const;
    ITraversable* traversable() const { return currentTraversable_; }
    bool hasReachedDestination() const { return hasReachedDestination_; }

private:
    void updatePositionAndAngle();
    qreal distanceToStopLine();
    void updateStoppingPoint();

    void applyPhysics(qreal deltaTime);
    void updateDecision();

    bool canSafelyProceed(const ConflictData& decisionData) const;
    bool isSufficientlyAheadOf(qreal thisApproachTime, const PriorityAgentInfo& other) const;
    qreal calculateTimeToReach(qreal distance, qreal initialSpeed, qreal acceleration, qreal maxSpeed) const;
    qreal calculateDistanceToConflict(const ConflictData& conflict) const;
    void setNextDrivingBehavior(bool canProceed);
    VehiclePID::Input pidInput(qreal deltaTime);
};

*/
