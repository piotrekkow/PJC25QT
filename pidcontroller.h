#pragma once
#include <qtypes.h>

/**
 * @brief PID Controller implementation.
 * Requires external output clamping.
 * Remember to reset before use.
 */
class PIDController
{
    qreal kp_, ki_, kd_;
    qreal previousError_;
    qreal integral_;

public:
    PIDController(qreal kp, qreal ki, qreal kd);
    PIDController(qreal kp, qreal ki);

    qreal update(qreal setpoint, qreal currentValue, qreal deltaTime);
    void reset();
    void gains(qreal kp, qreal ki, qreal kd) { kp_ = kp; ki_ = ki; kd_ = kd; }
};
