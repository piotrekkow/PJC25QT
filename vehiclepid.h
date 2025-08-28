#pragma once
#include <qtypes.h>

enum class DrivingBehavior {
    StopAtPoint,
    FollowVehicle,
    AdaptiveCruise,
    EmergencyBrake
};

class VehiclePID
{
public:
    struct Input {
        DrivingBehavior action;
        qreal currentSpeed;
        qreal deltaTime;

        // Scenario-specific data
        qreal desiredCruiseSpeed;

        // Stop scenario
        qreal distanceToStopPoint = 0.0;

        // Following scenario
        qreal leadVehicleSpeed = 0.0;
        qreal leadVehicleDistance = 0.0;
        qreal leadVehicleAcceleration = 0.0; // Optional: for predictive following

        const qreal maxAcceleration;
        const qreal comfortableDeceleration;
        const qreal maxDeceleration;

        const qreal safetyTimeGap;
        const qreal safetyMinDistance;
    };

private:
    qreal kp_;
    qreal ki_;
    qreal kd_;
    qreal integral_;
    qreal previousError_;
    qreal previousAccel_; // for rate limiting
    bool firstRun_;

public:
    VehiclePID();
    VehiclePID(qreal kp, qreal ki, qreal kd);

    qreal calculateAcceleration(const Input& input);
    void gains(qreal kp, qreal ki, qreal kd);
    void reset();

private:
    qreal applyPIDControl(qreal targetSpeed, const Input& input);
    qreal calculateTargetSpeed(const Input& input);
    qreal applyHumanLikeConstraints(qreal requestedAccel, const Input& input);
    qreal calculateSituationFactor(const Input& input);
    qreal applyRateLimiting(qreal requestedAccel, qreal deltaTime);
    qreal calculateStopTargetSpeed(const Input& input);
    qreal calculateAdaptiveTargetSpeed(const Input& input);
};
