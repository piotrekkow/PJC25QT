#include "drivermodel.h"
#include "vehicle.h"

qreal DriverModel::desiredAcceleration(const Vehicle* vehicle, DecisionContext context, qreal deltaTime)
{
    bool stopAtIntersection = shouldStopAtIntersection(vehicle, context);
    bool followBehind = shouldFollowVehicle(vehicle, context);
    bool queueBehind = shouldQueueBehindVehicle(vehicle, context);

    if (stopAtIntersection || queueBehind)
    {
        action_ = queueBehind ? DriverAction::Queueing : DriverAction::Stopping;

        if (action_ == DriverAction::Stopping && vehicle->speed() < 0.5)
            action_ = DriverAction::Stopped;

        return queueBehind ? queueingAcceleration(vehicle, context, deltaTime)
                           : stoppingAcceleration(vehicle, context, deltaTime);
    }
    else if (followBehind)
    {
        action_ = DriverAction::Following;
        return followingAcceleration(vehicle, context, deltaTime);
    }
    else
    {
        action_ = DriverAction::Proceeding;
        return proceedingAcceleration(vehicle, context, deltaTime);
    }
}

qreal DriverModel::stoppingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
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

    qreal kp = 5.0;
    controller_.gains(kp, 0.0, 0.0);

    qreal stopDistanceBuffer = vehicle->speed() * 0.0;
    // qreal safeSpeed = std::sqrt(2.0 * std::max(0.0, effectiveStopDistance - stopDistanceBuffer) * comfDecel_);

    qreal responseBuffer = vehicle->speed() * 0.3;
    qreal bufferedSafeSpeed = std::sqrt(2.0 * std::max(0.0, effectiveStopDistance - stopDistanceBuffer - responseBuffer) * comfDecel_);

    qreal transitionRange = 2.0;
    qreal speedDiff = vehicle->speed() - bufferedSafeSpeed;

    qreal setpoint;
    if (speedDiff <= 0)
    {
        setpoint = bufferedSafeSpeed;
    }
    else if (speedDiff >= transitionRange)
    {
        setpoint = 0.0;
    }
    else
    {
        qreal t = speedDiff / transitionRange;
        qreal smooth_t = t * t * (3.0 - 2.0 * t);
        setpoint = bufferedSafeSpeed * (1.0 - smooth_t);
    }

    return controller_.update(setpoint, vehicle->speed(), deltaTime);
}

qreal DriverModel::queueingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
{
    return stoppingAcceleration(vehicle, context, deltaTime);
}

qreal DriverModel::followingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
{
    auto lead = context.leadVehicle;
    qreal relativeDistance = lead->progress() - lead->length() - vehicle->progress();
    qreal relativeSpeed = vehicle->speed() - lead->speed();

    // Using the Intelligent Driver Model (IDM) formula for safe distance
    qreal safeDistance = minDistanceGap_ + std::max(0.0, vehicle->speed() * minTimeGap_ + (vehicle->speed() * relativeSpeed) / (2 * std::sqrt(comfAccel_ * comfDecel_)));
    controller_.gains(1.0, 0.05, 0.2);
    qreal setpoint = (relativeDistance > safeDistance) ? context.speedLimit
                                                 : lead->speed();

    return controller_.update(setpoint, vehicle->speed(), deltaTime);
}

qreal DriverModel::proceedingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
{
    controller_.gains(1.0, 0.05, 0.7);
    qreal setpoint = context.speedLimit;
    return controller_.update(setpoint, vehicle->speed(), deltaTime);
}

qreal DriverModel::decisionDistance(const Vehicle *vehicle, DecisionContext &context) const
{
    Q_UNUSED(context);

    // allows for a very comfortable margin for stopping at 1/2 comfDeceleration_, reasonable substitute for infinity
    return (vehicle->speed() * vehicle->speed() / comfDecel_) + 2 * minDistanceGap_;
}

bool DriverModel::shouldStopAtIntersection(const Vehicle *vehicle, DecisionContext &context) const
{
    return context.distanceToStop < decisionDistance(vehicle, context);
}

bool DriverModel::shouldFollowVehicle(const Vehicle *vehicle, DecisionContext &context) const
{
    if (!context.leadVehicle) return false;

    auto lead = context.leadVehicle;
    qreal relativeDistance = lead->progress() - lead->length() - vehicle->progress();
    return relativeDistance < decisionDistance(vehicle, context);
}

bool DriverModel::shouldQueueBehindVehicle(const Vehicle *vehicle, DecisionContext &context) const
{
    if (shouldFollowVehicle(vehicle, context))
    {
        return context.leadVehicle->speed() < 0.25 * context.speedLimit;
    }
    else
        return false;
}
