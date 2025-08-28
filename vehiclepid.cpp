#include "vehiclepid.h"
#include "vehicle.h"
#include <QDebug>

VehiclePID::VehiclePID(const Vehicle *vehicle, qreal kp, qreal ki, qreal kd)
    : vehicle_{ vehicle }
    , mode_{ Mode::Cruising }
    , kp_{ kp }
    , ki_{ ki }
    , kd_{ kd }
{}

VehiclePID::VehiclePID(const Vehicle *vehicle)
    : VehiclePID(vehicle, 1.0, 0.1, 0.2)
{}

void VehiclePID::gains(qreal kp, qreal ki, qreal kd)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

void VehiclePID::mode(Mode mode)
{
    if (mode != mode_)
    {
        mode_ = mode;
        reset();
    }
}

void VehiclePID::reset()
{
    integral_ = 0.0;
    previousError_ = 0.0;
    qDebug() << "PID reset";
}

qreal VehiclePID::update(qreal distanceToTarget, qreal deltaTime, qreal targetSpeed)
{
    qreal error;
    qreal desiredSpeed;
    qreal currentSpeed = vehicle_->speed();

    switch (mode_)
    {
        case Cruising:
        {
            error = targetSpeed - currentSpeed;
            return kp_ * error;
        }

        case Following:
        {
            qreal safeDistance = currentSpeed * SAFETY_TIME_GAP + SAFETY_MIN_DISTANCE;
            qreal distanceError = distanceToTarget - safeDistance;
            qreal distanceGain = 0.5;
            desiredSpeed = currentSpeed + (distanceGain * distanceError);
            error = desiredSpeed - currentSpeed;
        }
        break;

        case Stopping:
        {
            qreal decelerationFactor = 1.0;
            desiredSpeed = std::max(0.0, decelerationFactor * std::sqrt(std::max(0.0, distanceToTarget)));
            desiredSpeed = std::min(desiredSpeed, currentSpeed);

            error = desiredSpeed - currentSpeed;
        }
        break;
    }

    integral_ += error * deltaTime;
    qreal derivative = (error - previousError_) / deltaTime;
    qreal output = (kp_ * error) + (ki_ * integral_) + (kd_ * derivative);

    return std::clamp(output, -1.0, 1.0);
}
