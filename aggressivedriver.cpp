#include "aggressivedriver.h"
#include "vehicle.h"

qreal AggressiveDriver::followingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
{
    auto lead = context.leadVehicle;
    qreal relativeDistance = lead->progress() - lead->length() - vehicle->progress();
    qreal aggressiveSafeDistance = minDistanceGap_ + vehicle->speed() * minTimeGap_ * 0.8;

    controller_.gains(1.5, 0.1, 0.3);

    qreal setpoint = (relativeDistance > aggressiveSafeDistance) ? context.speedLimit * 1.1
                                                                 : lead->speed() * 1.05;

    return controller_.update(setpoint, vehicle->speed(), deltaTime);
}

qreal AggressiveDriver::proceedingAcceleration(const Vehicle *vehicle, DecisionContext &context, qreal deltaTime)
{
    controller_.gains(1.2, 0.01, 0.6);
    qreal setpoint = 1.1 * context.speedLimit;
    return controller_.update(setpoint, vehicle->speed(), deltaTime);
}

qreal AggressiveDriver::decisionDistance(const Vehicle *vehicle, DecisionContext &context) const
{
    Q_UNUSED(context);
    return (vehicle->speed() * vehicle->speed() / 1.5 * comfDecel_) + 2 * minDistanceGap_;
}
