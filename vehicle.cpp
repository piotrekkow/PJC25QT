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
{
    driver_ = std::make_unique<DefaultDriver>();
}

std::unique_ptr<Vehicle> Vehicle::create(Lane *initialLane, const Traffic *traffic, const GeometryManager *geometry)
{
    auto vehicle = std::unique_ptr<Vehicle>(new Vehicle(initialLane, traffic, geometry));
    vehicle->navigationStrategy_ = initialLane->createNavigationStrategy(vehicle.get(), vehicle->traffic_, vehicle->geometry_);
    return vehicle;
}

void Vehicle::applyPhysics(qreal deltaTime)
{
    speed_ += acceleration_ * deltaTime;

    // const qreal stoppingSpeedThreshold = 0.1;

    // if ((driver_->action() == DriverAction::Stopping || driver_->action() == DriverAction::Queueing) && speed_ < stoppingSpeedThreshold)
    // {
    //     speed_ = 0.0;
    //     acceleration_ = 0.0;
    // }

    if (speed_ < 0.5 && acceleration_ < 0)
    {
        speed_ = 0.0;
        acceleration_ = 0.0;
        driver_->reset();

    }

    Agent::applyPhysics(deltaTime);
}

void Vehicle::updateDynamics(qreal deltaTime)
{
    qreal desiredAccel = (driver_) ? driver_->desiredAcceleration(this, decisionContext(), deltaTime)
                                   : 0.0;

    applyAccelerationLimits(desiredAccel, deltaTime);
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

void Vehicle::applyAccelerationLimits(qreal desiredAcceleration, qreal deltaTime)
{
    qreal maxAccelChange = maxJerk_ * deltaTime;
    qreal reqAccelChange = desiredAcceleration - acceleration_;

    reqAccelChange = std::clamp(reqAccelChange, -maxAccelChange, maxAccelChange);
    acceleration_ += reqAccelChange;
    acceleration_ = std::clamp(acceleration_, -maxDeceleration_, maxAcceleration_);
}

const Vehicle *Vehicle::getLeadVehicle() const
{
    const Agent* leadAgent = traffic_->findLeadVehicle(this);
    return (leadAgent) ? static_cast<const Vehicle*>(leadAgent) : nullptr;
}

qreal Vehicle::timeToReach(qreal distance)
{
    // if (std::abs(acceleration_) < 0.1) return (speed_ > 0) ? distance / speed_ : std::numeric_limits<qreal>::max();

    qreal accelDistance = (cruiseSpeed_ * cruiseSpeed_ - speed_ * speed_) / (2 * maxAcceleration_); // from stop at a=1.8 and v_cruise=13.8 => 52.9m

    if (accelDistance > distance)
    {
        // solve for t: d = v0t + 1/2at^2 => t = (-v + sqrt(v^2 + 2ad)) / a
        return (-speed_ + std::sqrt(speed_ * speed_ + 2 * maxAcceleration_ * distance)) / maxAcceleration_;
    }
    else
    {
        qreal accelTime = (cruiseSpeed_ - speed_) / maxAcceleration_;
        qreal cruiseDistance = distance - accelDistance;
        qreal cruiseTime = cruiseDistance / cruiseSpeed_;
        return accelTime + cruiseTime;
    }
}

DecisionContext Vehicle::decisionContext() const
{
    return {
        .leadVehicle = traffic_->findLeadVehicle(this),
        .distanceToStop = navigationStrategy_->distanceToStop(),
        .speedLimit = cruiseSpeed_,
        .acceleration = acceleration_,
        .maxAcceleration = maxAcceleration_,
        .maxDeceleration = maxDeceleration_,
        .maxJerk = maxJerk_
    };
}
