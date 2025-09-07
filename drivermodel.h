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

/**
 * @brief Decision making module for the vehicle.
 * Driver model acts as a base class implementation and allows for independent logic changes by base classes to any of the actions performed by a driver
 */
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

    /**
     * @brief calculates desired acceleration based on driver perception
     * @param vehicle driven by the driver
     * @param context currently surrounding the vehicle, creted in vehicle class
     * @param deltaTime
     * @return acceleration in m/s^2, not limited, without jerk applied
     */
    qreal desiredAcceleration(const Vehicle* vehicle, DecisionContext context, qreal deltaTime);

    /**
     * @return action currently performed by the driver
     */
    DriverAction action() const { return action_; }

    /**
     * @brief reset the pid controller
     */
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
