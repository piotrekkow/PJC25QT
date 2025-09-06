#pragma once
#include "drivermodel.h"

class AggressiveDriver : public DriverModel
{
public:
    AggressiveDriver()
        : DriverModel(1.5, 1.0, 2.0, 3.0)
    {};

protected:
    qreal followingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime) override;
    qreal proceedingAcceleration(const Vehicle* vehicle, DecisionContext& context, qreal deltaTime) override;
    qreal decisionDistance(const Vehicle* vehicle, DecisionContext& context) const override;
};
