#pragma once
#include <qtypes.h>

class RoadNetwork;
class Vehicle;
class Lane;

class TrafficManager
{
    RoadNetwork* network_;

public:
    TrafficManager(RoadNetwork* network);

    void update(qreal deltaTime);

    Vehicle* createVehicle(Lane* initialLane);
    void removeVehicle(Vehicle* vehicle);
};
