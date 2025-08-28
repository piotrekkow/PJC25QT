#pragma once

#include "IntersectionDecisionData.h"
#include "vehiclepid.h"
#include <QPointF>
#include <QColor>

class Lane;
class GeometryManager;
class ITraversable;
class TrafficManager;
class VehiclePID;
struct ConflictData;

class Vehicle
{
    enum class Decision
    {
        StoppingAtStopSign,
        Yielding,
        Proceeding
    };

    const GeometryManager* networkGeometry_;
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
    qreal comfortableDeceleration_;  // m/s^2, e.g., 2.5 (used for normal yielding)
    qreal maxDeceleration_;          // m/s^2, e.g., 8.0 (used for emergency stops)

    Decision decision_;
    DrivingBehavior behavior_;
    const TrafficManager* trafficManager_;
    VehiclePID pidController_;
    qreal safetyTimeGap_{ 2.0 };
    qreal safetyMinDistance_{ 1.5 };
    qreal nextStopDistance_;

public:
    Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, TrafficManager* trafficManager, qreal initialPosition = 0.0);
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

    void applyPhysics(qreal deltaTime);
    void updateDecision();

    bool canSafelyProceed(const IntersectionDecisionData& decisionData) const;
    bool isSufficientlyAheadOf(qreal thisApproachTime, const PriorityVehicleInfo& other) const;
    qreal calculateTimeToReach(qreal distance, qreal initialSpeed, qreal acceleration, qreal maxSpeed) const;
    qreal calculateDistanceToConflict(const ConflictData& conflict) const;
    void setNextDrivingBehavior(bool canProceed);
    VehiclePID::Input pidInput(qreal deltaTime);

};
