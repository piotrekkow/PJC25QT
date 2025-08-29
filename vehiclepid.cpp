#include "vehiclepid.h"
#include "vehicle.h"
#include <QDebug>

VehiclePID::VehiclePID(qreal kp, qreal ki, qreal kd, qreal kpStop, qreal kdStop, qreal kpCruise, qreal kdCruise)
    : kp_{ kp }
    , ki_{ ki }
    , kd_{ kd }
    , kpStop_{ kpStop }
    , kdStop_{ kdStop }
    , kpCruise_{ kpCruise }
    , kdCruise_{ kdCruise }
    , previousReferenceSpeed_{ 13.89 }
    , previousEgoSpeed_{ 13.89 }
{}

VehiclePID::VehiclePID()
    : VehiclePID(8.0, 0.4, 0.02, 0.6, 0.4, 0.7, 0.4)
{}

void VehiclePID::gains(qreal kp, qreal ki, qreal kd, qreal kpStop, qreal kdStop, qreal kpCruise, qreal kdCruise)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    kpStop_ = kpStop;
    kdStop_ = kdStop;
    kpCruise_ = kpCruise;
    kdCruise_ = kdCruise;
}

qreal VehiclePID::calculateAcceleration(const Input &input)
{
    qreal reqFollowingAccel, reqStoppingAccel, gapError, referenceSpeed;

    // PID following controler
    if (input.egoLeaderGap < input.egoSpeed * input.egoSpeed)
    {
        qreal gapDesired = input.comfDistanceGap + input.comfTimeGap * input.leaderSpeed;
        gapError = input.egoLeaderGap - gapDesired;
        qreal relativeSpeed = input.egoSpeed - input.leaderSpeed;

        reqFollowingAccel = kp_ * gapError - kd_ * relativeSpeed + ki_ * integral_;
    }
    else
    {
        reqFollowingAccel = std::numeric_limits<qreal>::max();
        gapError = 0;
    }

    // PD stop controller
    if (input.distanceToStop < input.egoSpeed * input.egoSpeed)
    {
        // qreal effectiveDecel = 0.7 * input.comfDeceleration; // coefficient in order to counteract jerk and inertia
        qreal buffer = 0.5 + 1.2 * input.egoSpeed;
        referenceSpeed = std::sqrt(std::max(0.0, 2 * input.comfDeceleration * std::max(0.0, input.distanceToStop - buffer)));
        qreal speedError = referenceSpeed - input.egoSpeed;
        qreal accelerationError = (referenceSpeed - previousReferenceSpeed_) / input.deltaTime - (input.egoSpeed - previousEgoSpeed_) / input.deltaTime;
        reqStoppingAccel = kpStop_ * speedError + kdStop_ * accelerationError;
    }
    else
    {
        referenceSpeed = input.egoSpeed;
        reqStoppingAccel = std::numeric_limits<qreal>::max();
    }

    // PD cruise control controller
    qreal speedError = input.egoDesiredSpeed - input.egoSpeed;
    qreal derivative = (-input.egoSpeed + previousEgoSpeed_) / input.deltaTime;
    qreal reqCruiseAccel = kpCruise_ * speedError + kdCruise_ * derivative;
    reqCruiseAccel = std::min(reqCruiseAccel, input.comfAcceleration);

    previousEgoSpeed_ = input.egoSpeed;
    previousReferenceSpeed_ = referenceSpeed;
    integral_ += gapError * input.deltaTime;
    std::clamp(integral_, -10.0, 10.0);

    qreal reqAccel = std::min({reqFollowingAccel, reqStoppingAccel, reqCruiseAccel});
    return applyJerkAndLimits(input, reqAccel);
}

qreal VehiclePID::applyJerkAndLimits(const Input &input, qreal& reqAccel)
{
    qreal maxChange = (reqAccel > input.egoAcceleration) ? input.accelJerk * input.deltaTime
                                                         : input.decelJerk * input.deltaTime;

    reqAccel = std::clamp(reqAccel,
                          input.egoAcceleration - maxChange,
                          input.egoAcceleration + maxChange);

    return std::clamp(reqAccel, -input.maxDeceleration, input.maxAcceleration);
}
