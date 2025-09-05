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
    Queueing
};

class DriverModel
{
protected:
    const qreal minTimeGap_;        // s
    const qreal minDistanceGap_;    // m

    const qreal comfAccel_;   // m/s^2
    const qreal comfDecel_;   // - m/s^2

    DriverAction action_;

public:
    virtual ~DriverModel() = default;
    virtual qreal desiredAcceleration(const Vehicle* vehicle, DecisionContext context, qreal deltaTime) = 0;
    DriverAction action() const { return action_; }
    virtual void reset() = 0;

protected:
    DriverModel(qreal minTimeGap, qreal minDistanceGap, qreal comfAccel, qreal comfDecel)
        : minTimeGap_{ minTimeGap }
        , minDistanceGap_{ minDistanceGap }
        , comfAccel_{ comfAccel }
        , comfDecel_{ comfDecel }
    {}

    virtual qreal decisionDistance(const Vehicle* vehicle, DecisionContext& context) const = 0;
};

class DefaultDriver : public DriverModel
{
    PIDController controller_;

public:
    DefaultDriver()
        : DriverModel(2.0, 1.5, 1.8, 2.0)
        , controller_{1.0, 0.0, 0.0}
    {}

    qreal desiredAcceleration(const Vehicle* vehicle, DecisionContext context, qreal deltaTime) override;
    void reset() override { controller_.reset(); }

protected:
    qreal decisionDistance(const Vehicle* vehicle, DecisionContext& context) const override;
};
