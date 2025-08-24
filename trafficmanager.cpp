#include "trafficmanager.h"
#include <vector>
#include "roadnetwork.h"
#include <QDebug>

TrafficManager::TrafficManager(const RoadNetwork *network)
    : network_{ network }
{}

void TrafficManager::update(qreal deltaTime)
{
    for (const auto& vehicle : vehicles_)
    {
        vehicle->update(deltaTime);
        if (vehicle->hasReachedDestination())
            removeVehicle(vehicle.get());
    }

}

Vehicle *TrafficManager::createVehicle(Lane *initialLane, qreal initialPosition)
{
    vehicles_.emplace_back(std::make_unique<Vehicle>(network_->geometry(), initialLane, initialPosition));
    return vehicles_.back().get();
}

void TrafficManager::removeVehicle(Vehicle *vehicle)
{
    vehicles_.erase(std::remove_if(vehicles_.begin(), vehicles_.end(),
                                   [vehicle](const std::unique_ptr<Vehicle>& v)
                                   {
                                       return v.get() == vehicle;
                                   }), vehicles_.end());
    qDebug() << "vehicle removed";
}
