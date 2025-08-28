#pragma once

#include "IntersectionDecisionData.h"
#include "vehiclepid.h"
#include <QPointF>
#include <QColor>

class Lane; // Forward declaration to avoid circular dependencies
class Connection;
class GeometryManager;
class ITraversable;
class TrafficManager;
class VehiclePID;
struct ConflictData;

class Vehicle
{
public:
    enum class VehicleState
    {
        Proceeding,      // Normal driving, following speed limits and other cars.
        Yielding,     // Decelerating to stop before a conflict point or stop line.
        Stopping,     // Performing a final, hard stop to 0 speed.
        Waiting,      // Stopped at a line, waiting for clearance.
        Queued        // Stopped behind another vehicle.
    };

    Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, TrafficManager* trafficManager, qreal initialPosition = 0.0);
    void update(qreal deltaTime);

    // --- Getters ---
    QPointF position() const { return position_; }
    qreal angle() const { return angle_; }
    QColor color() const { return color_; }
    qreal length() const { return length_; }
    qreal width() const { return width_; }
    qreal progress() const { return progress_; }
    qreal speed() const { return currentSpeed_; }
    qreal acceleration() const { return currentAcceleration_; }
    qreal decisionDistance() const { return std::max(currentSpeed_ * currentSpeed_ / comfortableDeceleration_, 40.0); } // m, how far from a decision point to start thinking about it.
    ITraversable* traversable() const { return currentTraversable_; }
    bool hasReachedDestination() const { return hasReachedDestination_; }

private:
    void updatePositionAndAngle();
    void stopping(qreal distanceToStopPoint, qreal deltaTime);
    qreal distanceToStopLine();

    const GeometryManager* networkGeometry_;
    ITraversable* currentTraversable_;
    qreal progress_;
    bool hasReachedDestination_;

    qreal currentSpeed_; // in m/s
    qreal targetSpeed_;  // in m/s
    qreal currentAcceleration_; // in m/s^2

    qreal angle_;      // in degrees
    QPointF position_;

    QColor color_;
    qreal length_;
    qreal width_;

    qreal maxAcceleration_;          // m/s^2, e.g., 3.0
    qreal comfortableDeceleration_;  // m/s^2, e.g., 2.5 (used for normal yielding)
    qreal maxDeceleration_;          // m/s^2, e.g., 8.0 (used for emergency stops)

    VehicleState state_;
    const TrafficManager* trafficManager_;

    void applyPhysics(qreal deltaTime);
    void updateDecision();

    bool canSafelyProceed(const IntersectionDecisionData& decisionData) const;
    bool isSufficientlyAheadOf(qreal thisApproachTime, const PriorityVehicleInfo& other) const;
    qreal calculateTimeToReach(qreal distance, qreal initialSpeed, qreal acceleration, qreal maxSpeed) const;
    qreal calculateDistanceToConflict(const ConflictData& conflict) const;
    void setNextState(bool canProceed);

    // --- PID Controller Gains (these will need tuning) ---
    qreal Kp_ = 1.0; // Proportional gain - main braking force
    qreal Ki_ = 0.0; // Integral gain - corrects steady-state error
    qreal Kd_ = 0.0;  // Derivative gain - dampens and smooths

    // --- PID State Variables ---
    qreal integralError_ = 0.0;
    qreal previousError_ = 0.0;

    VehiclePID pidController_;
};
