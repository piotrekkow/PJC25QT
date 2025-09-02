#include "pidcontroller.h"
#include <algorithm> // clamp
#include <QDebug>

PIDController::PIDController(qreal kp, qreal ki, qreal kd)
    : kp_{ kp }
    , ki_{ ki }
    , kd_{ kd }
{}

PIDController::PIDController(qreal kp, qreal ki)
    : PIDController(kp, ki, 0.0)
{}

qreal PIDController::update(qreal setpoint, qreal currentValue, qreal deltaTime)
{
    qreal error = setpoint - currentValue;
    integral_ += error * deltaTime;
    integral_ = std::clamp(integral_, -10.0, 10.0);
    qreal derivative = (deltaTime > 0) ? kd_ * (error - previousError_) / deltaTime
                                 : 0.0;

    previousError_ = error;
    return kp_ * error + ki_ * integral_ + derivative;
}

void PIDController::reset()
{
    previousError_ = 0.0;
    integral_ = 0.0;
}



