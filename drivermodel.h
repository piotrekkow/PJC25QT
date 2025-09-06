#pragma once
#include <qtypes.h>
#include "pidcontroller.h"

class Vehicle;
struct DecisionContext;

enum class DriverAction
{
    Stopping,
    Proceeding,
    Following,
    Queueing,
    Stopped
};

class DriverModel
{
protected:
    const qreal minTimeGap_;        // s
    const qreal minDistanceGap_;    // m

    const qreal comfAccel_;   // m/s^2
    const qreal comfDecel_;   // - m/s^2

    DriverAction action_;
    PIDController controller_;

public:
    virtual ~DriverModel() = default;
    qreal desiredAcceleration(const Vehicle* vehicle, DecisionContext context, qreal deltaTime);

    DriverAction action() const { return action_; }
    void reset() { controller_.reset(); }

protected:
    DriverModel(qreal minTimeGap, qreal minDistanceGap, qreal comfAccel, qreal comfDecel)
        : minTimeGap_{ minTimeGap }
        , minDistanceGap_{ minDistanceGap }
        , comfAccel_{ comfAccel }
        , comfDecel_{ comfDecel }
        , controller_{1.0, 0.0, 0.0}
    {}

    virtual qreal stoppingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime);
    virtual qreal queueingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime);
    virtual qreal followingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime);
    virtual qreal proceedingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime);

    // Decision-making methods
    virtual qreal decisionDistance(const Vehicle* vehicle, DecisionContext& context) const;
    virtual bool shouldStopAtIntersection(const Vehicle* vehicle, DecisionContext& context) const;
    virtual bool shouldFollowVehicle(const Vehicle* vehicle, DecisionContext& context) const;
    virtual bool shouldQueueBehindVehicle(const Vehicle* vehicle, DecisionContext& context) const;
};

class AverageDriver : public DriverModel
{
public:
    AverageDriver()
        : DriverModel(2.0, 1.5, 2.0, 2.0)
    {}
};
