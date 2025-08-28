#include "vehiclepid.h"
#include "vehicle.h"
#include <QDebug>

VehiclePID::VehiclePID(qreal kp, qreal ki, qreal kd)
    : kp_{ kp }
    , ki_{ ki }
    , kd_{ kd }
    , previousAccel_{ 0.0 }
    , firstRun_{ true }
{}

VehiclePID::VehiclePID()
    : VehiclePID(2.0, 0.1, 0.5)
{}

void VehiclePID::gains(qreal kp, qreal ki, qreal kd)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

qreal VehiclePID::calculateAcceleration(const Input &input)
{
    qreal targetSpeed = calculateTargetSpeed(input);
    return applyPIDControl(targetSpeed, input);
}

qreal VehiclePID::applyPIDControl(qreal targetSpeed, const Input &input)
{
    if (firstRun_)
    {
        reset();
        firstRun_ = false;
    }

    // PID error calculation
    qreal speedError = targetSpeed - input.currentSpeed;

    // Update integral with windup protection
    integral_ += speedError * input.deltaTime;
    integral_ = std::clamp(integral_, -10.0, 10.0); // Prevent windup

    // Calculate derivative
    qreal derivative = 0.0;
    if (input.deltaTime > 0.001) { // Avoid division by zero
        derivative = (speedError - previousError_) / input.deltaTime;
    }

    // PID output
    qreal pidOutput = kp_ * speedError + ki_ * integral_ + kd_ * derivative;

    // Apply human-like constraints and smoothing
    qreal finalAccel = applyHumanLikeConstraints(pidOutput, input);

    // Update previous error for next iteration
    previousError_ = speedError;

    return finalAccel;
}

qreal VehiclePID::calculateTargetSpeed(const Input &input)
{
    switch (input.action)
    {
        case DrivingBehavior::StopAtPoint:
            return calculateStopTargetSpeed(input);

        case DrivingBehavior::FollowVehicle:
        //    return calculateFollowTargetSpeed(input);

        case DrivingBehavior::AdaptiveCruise:
            return calculateAdaptiveTargetSpeed(input);

        case DrivingBehavior::EmergencyBrake:
            return 0.0;
    }
    return input.desiredCruiseSpeed;
}

qreal VehiclePID::applyHumanLikeConstraints(qreal requestedAccel, const Input &input)
{
    // Determine situation-based limits
    qreal situationFactor = calculateSituationFactor(input);

    // Comfortable limits based on situation
    qreal maxComfortAccel = input.maxAcceleration * (0.6 + 0.4 * situationFactor);
    qreal maxComfortDecel = input.comfortableDeceleration * (0.8 + 0.2 * situationFactor);

    // Check for emergency conditions
    if (input.action == DrivingBehavior::EmergencyBrake) {
        return input.maxDeceleration;
    }

    // Apply smooth rate limiting (humans don't make instant changes)
    requestedAccel = applyRateLimiting(requestedAccel, input.deltaTime);

    // Final constraint to comfort/safety limits
    return std::clamp(requestedAccel, maxComfortDecel, maxComfortAccel);
}

qreal VehiclePID::calculateSituationFactor(const Input &input)
{
    // Factor representing urgency/distance (0.0 = close/urgent, 1.0 = far/relaxed)
    qreal distanceFactor = 1.0;

    // Consider stop point distance
    if (input.action == DrivingBehavior::StopAtPoint) {
        distanceFactor = std::min(distanceFactor,
                                  std::min(1.0, input.distanceToStopPoint / 100.0));
    }

    // Consider lead vehicle distance
    if (input.action == DrivingBehavior::FollowVehicle) {
        qreal desiredFollowDistance = input.currentSpeed * 2.0; // 2-second rule
        distanceFactor = std::min(distanceFactor,
                                  std::min(1.0, input.leadVehicleDistance / desiredFollowDistance));
    }

    return distanceFactor;
}

qreal VehiclePID::applyRateLimiting(qreal requestedAccel, qreal deltaTime)
{
    // Human reaction/comfort limit - can't change acceleration instantly
    qreal maxAccelChangeRate = 4.0; // m/s² per second
    qreal maxChange = maxAccelChangeRate * deltaTime;

    qreal limitedAccel = std::clamp(requestedAccel,
                                    previousAccel_ - maxChange,
                                    previousAccel_ + maxChange);

    previousAccel_ = limitedAccel;
    return limitedAccel;
}

qreal VehiclePID::calculateStopTargetSpeed(const Input &input)
{
    // If we are very close to the stop point, the target is 0 to prevent overshooting.
    const qreal stopThreshold = 0.3; // meters
    if (input.distanceToStopPoint < stopThreshold) {
        return 0.0;
    }

    // Calculate the ideal speed based on the braking curve.
    // Based on the kinematic equation: v_target = sqrt(2 * deceleration * distance)
    // We use max(0.0, ...) to prevent a domain error in sqrt if the distance is negative.
    qreal distance = std::max(0.0, input.distanceToStopPoint);
    qreal targetSpeed = std::sqrt(2.0 * input.comfortableDeceleration * distance);

    // The target speed should not exceed the driver's desired cruise speed.
    // The vehicle shouldn't speed up just to match a distant braking profile.
    targetSpeed = std::min(targetSpeed, input.desiredCruiseSpeed);

    // Final safety check to ensure we don't return a negative speed.
    return std::max(0.0, targetSpeed);
}

qreal VehiclePID::calculateAdaptiveTargetSpeed(const Input &input)
{
    qreal stopTarget = calculateStopTargetSpeed(input);
    // qreal followTarget = calculateFollowTargetSpeed(input);

    return stopTarget;
    // return std::min(stopTarget, followTarget)
}

void VehiclePID::reset() {
    integral_ = 0.0;
    previousError_ = 0.0;
    firstRun_ = true;
}
