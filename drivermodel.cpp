#include "drivermodel.h"
#include "vehicle.h"

qreal DefaultDriver::desiredAcceleration(const Vehicle *vehicle, DecisionContext context, qreal deltaTime)
{
    qreal reqAccel = 0.0;
    qreal setpoint = 0.0;

    bool stopAtIntersection = context.distanceToStop < decisionDistance(vehicle, context);
    bool followBehindVehicle = false;
    bool queueBehindVehicle = false;

    if (auto& lead = context.leadVehicle)
    {
        qreal relativeDistance = lead->progress() - lead->length() - vehicle->progress();
        followBehindVehicle = relativeDistance < decisionDistance(vehicle, context);

        if (followBehindVehicle)
        {
            queueBehindVehicle = lead->speed() < 0.25 * context.speedLimit;
        }
    }

    // Priority 1: Must stop at an intersection or behind vehicle

    if (stopAtIntersection || queueBehindVehicle)
    {
        qreal effectiveStopDistance = context.distanceToStop - minDistanceGap_;

        if (auto& lead = context.leadVehicle)
        {
            qreal posLeadVehicleRear = lead->progress() - lead->length();
            qreal distanceToLeadRear = posLeadVehicleRear - vehicle->progress();

            if (distanceToLeadRear < effectiveStopDistance)
            {
                effectiveStopDistance = distanceToLeadRear - minDistanceGap_;
            }
        }

        // qreal kp = (effectiveStopDistance > 0.1) ? 4 / effectiveStopDistance + 1
        //                                          : 40.0;

        qreal kp = 5.0;

        controller_.gains(kp, 0.0, 0.0);
        action_ = (queueBehindVehicle) ? DriverAction::Queueing
                                       : DriverAction::Stopping;

        qreal stopDistanceBuffer = vehicle->speed() * 0.0;
        // qreal safeSpeed = std::sqrt(2.0 * std::max(0.0, effectiveStopDistance - stopDistanceBuffer) * comfDecel_);

        qreal responseBuffer = vehicle->speed() * 0.3; // Adjust based on your controller response time
        qreal bufferedSafeSpeed = std::sqrt(2.0 * std::max(0.0, effectiveStopDistance - stopDistanceBuffer - responseBuffer) * comfDecel_);

        // Smooth transition zone
        qreal transitionRange = 2.0; // m/s transition range
        qreal speedDiff = vehicle->speed() - bufferedSafeSpeed;

        if (speedDiff <= 0) {
            setpoint = bufferedSafeSpeed;
        } else if (speedDiff >= transitionRange) {
            setpoint = 0.0;
        } else {
            // Smooth cubic interpolation in transition zone
            qreal t = speedDiff / transitionRange;
            qreal smooth_t = t * t * (3.0 - 2.0 * t); // Smoothstep function
            setpoint = bufferedSafeSpeed * (1.0 - smooth_t);
        }

        if (action_ == DriverAction::Stopping && vehicle->speed() < 0.5)
            action_ = DriverAction::Stopped;

        reqAccel = controller_.update(setpoint, vehicle->speed(), deltaTime);
    }
    // Priority 2: should follow a vehicle
    else if (followBehindVehicle)
    {
        auto lead = context.leadVehicle;
        qreal relativeDistance = lead->progress() - lead->length() - vehicle->progress();

        if (relativeDistance < decisionDistance(vehicle, context))
        {
            qreal relativeSpeed = vehicle->speed() - lead->speed();

            // Using the Intelligent Driver Model (IDM) formula for safe distance
            qreal safeDistance = minDistanceGap_ + std::max(0.0, vehicle->speed() * minTimeGap_ + (vehicle->speed() * relativeSpeed) / (2 * std::sqrt(comfAccel_ * comfDecel_)));

            controller_.gains(1.0, 0.05, 0.2);
            action_ = DriverAction::Following;

            setpoint = (relativeDistance > safeDistance) ? context.speedLimit
                                                         : lead->speed();

            reqAccel = controller_.update(setpoint, vehicle->speed(), deltaTime);
        }
    }
    // Priority 3: cruise
    else
    {
        controller_.gains(1.0, 0.05, 0.7);
        action_ = DriverAction::Proceeding;
        setpoint = context.speedLimit;
        reqAccel = controller_.update(setpoint, vehicle->speed(), deltaTime);
    }

    return reqAccel;
}

qreal DefaultDriver::decisionDistance(const Vehicle* vehicle, DecisionContext &context) const
{
    Q_UNUSED(context);

    // allows for a very comfortable margin for stopping at 1/2 comfDeceleration_, reasonable substitute for infinity
    return (vehicle->speed() * vehicle->speed() / comfDecel_) + 2 * minDistanceGap_;
}
