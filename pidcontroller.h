#pragma once
#include <qtypes.h>

/**
 * @brief PID Controller implementation.
 * Requires external output clamping.
 */
class PIDController
{
    qreal kp_, ki_, kd_;
    qreal previousError_;
    qreal integral_;

public:
    PIDController(qreal kp, qreal ki, qreal kd);
    PIDController(qreal kp, qreal ki);

    qreal update(qreal setpoint, qreal currentValue, qreal deltaTime, qreal feedforward = 0.0);
    void reset();
    void gains(qreal kp, qreal ki, qreal kd) { kp_ = kp; ki_ = ki; kd_ = kd; }
};
