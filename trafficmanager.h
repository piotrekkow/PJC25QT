#pragma once
#include <qtypes.h>
#include "vehicle.h"

class RoadNetwork;
class Lane;

class TrafficManager
{
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
    const RoadNetwork* network_;


public:
    TrafficManager(const RoadNetwork* network);

    void update(qreal deltaTime);

    Vehicle* createVehicle(Lane* initialLane, qreal initialPosition = 0.0);
    const std::vector<std::unique_ptr<Vehicle>>& vehicles() const { return vehicles_; }
    void removeVehicle(Vehicle* vehicle);
};
