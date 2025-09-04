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
    , controller_(1.0, 0.1, 0.2)
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
    return speed_ * speed_ / 3;
}

void Vehicle::applyPhysics(qreal deltaTime)
{
    speed_ += acceleration_ * deltaTime;
    Agent::applyPhysics(deltaTime);
}

void Vehicle::updateDynamics(qreal deltaTime)
{
    qreal reqAccel = 0.0;
    qreal setpoint = 0.0;   // speed or decel

    // LeadVehicleData lead = getLeadVehicleData();
    // qreal safeDistance = minDistanceGap_ + speed_ * minTimeGap_ + (speed_ * lead.relativeSpeed) / (2 * comfDeceleration_);

    if (nextStopDistance_ > 0 && nextStopDistance_ < decisionDistance())
    {
        // Stop
        qreal stopKp = 10 / (speed_ + 5);
        controller_.gains(stopKp, 0.0, 0.2);

        qreal buffer = 1.5 * speed_;
        qreal targetSpeed = std::sqrt(2.0 * (nextStopDistance_ - buffer) * comfDeceleration_);
        setpoint = targetSpeed;
        reqAccel = controller_.update(setpoint, speed_, deltaTime);
        debugAction_ = DebugAction::Stopping;
    }
    else
    {
        // Cruise
        controller_.gains(1.0, 0.05, 0.7);
        setpoint = cruiseSpeed_;
        reqAccel = controller_.update(setpoint, speed_, deltaTime);
        debugAction_ = DebugAction::Proceeding;
    }

    qreal maxAccelChange = jerk_ * deltaTime;

    acceleration_ += (reqAccel > acceleration_) ? std::min(maxAccelChange, reqAccel - acceleration_)
                                                : std::max(-maxAccelChange, acceleration_ - reqAccel);

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
