#pragma once
#include <qtypes.h>

class Vehicle;

class VehiclePID
{
public:
    enum Mode
    {
        Cruising,
        Following,
        Stopping
    };

private:
    const Vehicle* vehicle_;
    Mode mode_;

    qreal kp_;
    qreal ki_;
    qreal kd_;

    qreal integral_;
    qreal previousError_;

    const qreal SAFETY_TIME_GAP{ 2.0 }; // s
    const qreal SAFETY_MIN_DISTANCE{ 1.5 }; // m

public:
    VehiclePID(const Vehicle* vehicle);
    VehiclePID(const Vehicle* vehicle, qreal kp, qreal ki, qreal kd);
    void gains(qreal kp, qreal ki, qreal kd);
    void mode(Mode mode);
    void reset();
    qreal update(qreal distanceToTarget, qreal deltaTime, qreal targetSpeed);
};
