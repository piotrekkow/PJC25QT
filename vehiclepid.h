#pragma once
#include <qtypes.h>

class VehiclePID
{
public:
    struct Input {
        qreal deltaTime;
        qreal egoSpeed;
        qreal egoAcceleration;
        qreal egoDesiredSpeed;
        qreal leaderSpeed;
        qreal egoLeaderGap;
        qreal distanceToStop;

        const qreal maxAcceleration;
        const qreal maxDeceleration;
        const qreal comfAcceleration;
        const qreal comfDeceleration;
        const qreal accelJerk;
        const qreal decelJerk;
        const qreal comfTimeGap;
        const qreal comfDistanceGap;
        const qreal pidDecisionCutoff;  // if vehicle is further than this it won't be considered for stopping
    };

private:
    qreal kp_;
    qreal ki_;
    qreal kd_;
    qreal kpStop_;
    qreal kdStop_;
    qreal kpCruise_;
    qreal kdCruise_;
    qreal integral_;
    qreal previousReferenceSpeed_;
    qreal previousEgoSpeed_;

public:
    VehiclePID();
    VehiclePID(qreal kp, qreal ki, qreal kd, qreal kpStop, qreal kdStop, qreal kpCruise, qreal kdCruise);

    qreal calculateAcceleration(const Input& input);
    void gains(qreal kp, qreal ki, qreal kd, qreal kpStop, qreal kdStop, qreal kpCruise, qreal kdCruise);
    void reset();

private:
    qreal applyJerkAndLimits(const Input& input, qreal& reqAccel);
};
