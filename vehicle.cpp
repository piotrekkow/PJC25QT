#include "vehicle.h"
#include "ConflictData.h"
#include "lane.h"
#include "geometrymanager.h"
#include "traffic.h"
#include <QDebug>

Vehicle::Vehicle(Lane *initialLane, const Traffic *traffic, const GeometryManager *geometry)
    : Agent(initialLane, traffic, geometry, 5.0, 2.0)
    , acceleration_{ 0.0 }
    , cruiseSpeed_{ initialLane->speedLimit() }
    , stoppingController_(1.0, 0.4, 0.02)
    , followingController_(0.6, 0.4)
    , cruiseController_(1.0, 0.5)
{}

std::unique_ptr<Vehicle> Vehicle::create(Lane *initialLane, const Traffic *traffic, const GeometryManager *geometry)
{
    auto vehicle = std::unique_ptr<Vehicle>(new Vehicle(initialLane, traffic, geometry));
    vehicle->navigationStrategy_ = initialLane->createNavigationStrategy(vehicle.get(), vehicle->traffic_, vehicle->geometry_);
    return vehicle;
}

qreal Vehicle::decisionDistance() const
{
    // allows for a very comfortable margin for stopping at 1 m/s^2, reasonable substitute for infinity
    return speed_ * speed_ / 2;
}

void Vehicle::applyPhysics(qreal deltaTime)
{
    speed_ += acceleration_ * deltaTime;
    Agent::applyPhysics(deltaTime);
}

void Vehicle::updateDynamics(qreal deltaTime)
{
    qreal reqAccel;
    // if (nextStopDistance_ < speed_ * speed_)
    // {
    //     qreal buffer = minDistanceGap_ + 1.2 * speed_;
    //     qreal referenceSpeed = std::sqrt(std::max(0.0, 2 * comfDeceleration_ * std::max(0.0, nextStopDistance_ - buffer)));
    //     reqAccel = stoppingController_.update(referenceSpeed, speed_, deltaTime);
    //     qDebug() << acceleration_;
    // }
    // else
    // {
    //     reqAccel = cruiseController_.update(cruiseSpeed_, speed_, deltaTime);
    //     qDebug() << this << ": a = " << acceleration_;
    // }
    reqAccel = cruiseController_.update(cruiseSpeed_, speed_, deltaTime);
    acceleration_ = std::clamp(reqAccel, -maxDeceleration_, maxAcceleration_);
}

std::unique_ptr<NavigationStrategy> Vehicle::createNavigationStrategyFor(const Traversable *newTraversable)
{
    if (const VehicleTraversable* vehTraversable = dynamic_cast<const VehicleTraversable*>(newTraversable))
    {
        return vehTraversable->createNavigationStrategy(this, traffic_, geometry_);
        cruiseSpeed_ = vehTraversable->speedLimit();
    }
    return nullptr;
}

qreal Vehicle::timeToReach(qreal distance)
{
    if (std::abs(acceleration_) < 0.1) return (speed_ > 0) ? distance / speed_ : std::numeric_limits<qreal>::max();

    qreal accelDistance = (cruiseSpeed_ * cruiseSpeed_ - speed_ * speed_) / (2 * comfAcceleration_); // from stop at a=1.8 and v_cruise=13.8 => 52.9m

    if (accelDistance > distance)
    {
        // solve for t: d = v0t + 1/2at^2 => t = (-v + sqrt(v^2 + 2ad)) / a
        return (-speed_ + std::sqrt(speed_ * speed_ + 2 * comfAcceleration_ * distance)) / comfAcceleration_;
    }
    else
    {
        qreal accelTime = (cruiseSpeed_ - speed_) / comfAcceleration_;
        qreal cruiseDistance = distance - accelDistance;
        qreal cruiseTime = cruiseDistance / cruiseSpeed_;
        return accelTime + cruiseTime;
    }
}
