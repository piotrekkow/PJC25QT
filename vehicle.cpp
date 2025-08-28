#include "vehicle.h"
#include "IntersectionDecisionData.h"
#include "lane.h"
#include "geometrymanager.h"
#include "intersection.h"
#include "trafficmanager.h"
#include <QDebug>

Vehicle::Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, TrafficManager* trafficManager, qreal initialPosition)
    : networkGeometry_{ networkGeometry }
    , currentTraversable_{ static_cast<ITraversable*>(initialLane) }
    , progress_{ initialPosition }
    , hasReachedDestination_{ false }
    , currentSpeed_{ 13.89 } // ~50 km/h
    , cruiseSpeed_{ 13.89 }
    , currentAcceleration_{ 0.0 }
    , angle_{ 0.0 }
    , color_{ Qt::blue }
    , length_{ 4.5 }
    , width_{ 1.8 }
    , maxAcceleration_{ 2.5 }
    , comfortableDeceleration_{ -2.0 }
    , maxDeceleration_{ -5.0 }
    , decision_{ Decision::Proceeding }
    , behavior_{ DrivingBehavior::AdaptiveCruise }
    , trafficManager_{ trafficManager }
    , pidController_{ VehiclePID() }
{
    updatePositionAndAngle();
}

void Vehicle::update(qreal deltaTime)
{
    updateDecision();

    qreal acceleration = pidController_.calculateAcceleration(pidInput(deltaTime));
    currentAcceleration_ = acceleration;

    applyPhysics(deltaTime);
    updatePositionAndAngle();
}

void Vehicle::updatePositionAndAngle()
{
    // Temporary logic for switching traversables before pathfinding and routing implementation
    if (progress_ >= currentTraversable_->length(networkGeometry_))
    {
        const auto& nextTraversables = currentTraversable_->next();
        if (!nextTraversables.empty())
        {
            progress_ -= currentTraversable_->length(networkGeometry_);
            currentTraversable_ = nextTraversables[0];
        }
        else
        {
            hasReachedDestination_ = true;
            return;
        }
    }
    //
    const QPainterPath* currentPath = &currentTraversable_->path(networkGeometry_);
    if (!currentPath || currentPath->isEmpty()) return;

    qreal pathLength = currentPath->length();
    qreal clampedProgress = std::max(0.0, std::min((qreal)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}

qreal Vehicle::distanceToStopLine()
{
    if (currentTraversable_->type() == ITraversable::TraversableType::Connection)
    {
        auto* conn = static_cast<const Connection*>(currentTraversable_);
        return conn->stopLineOffset() - progress_;
    }
    else
    {
        const auto& nextTraversables = currentTraversable_->next();
        if (nextTraversables.empty())
        {
            return std::numeric_limits<qreal>::max(); // No next stop line
        }
        qreal remainingLaneLength = currentTraversable_->length(networkGeometry_) - progress_;
        qreal nextConnectionStopOffset = static_cast<const Connection*>(nextTraversables[0])->stopLineOffset();
        return remainingLaneLength + nextConnectionStopOffset;
    }
}

void Vehicle::applyPhysics(qreal deltaTime)
{
    currentSpeed_ += currentAcceleration_ * deltaTime;
    currentSpeed_ = std::clamp(currentSpeed_, 0.0, cruiseSpeed_);
    progress_ += currentSpeed_ * deltaTime;
}

void Vehicle::updateDecision()
{
    auto* intersectionController = currentTraversable_->intersection()->controller();
    const IntersectionDecisionData decisionData = intersectionController->decisionData(this, trafficManager_->vehicles());

    bool canProceed = canSafelyProceed(decisionData);
    setNextDrivingBehavior(canProceed);
}

bool Vehicle::canSafelyProceed(const IntersectionDecisionData &decisionData) const
{
    for (const auto& conflict : decisionData.conflictsToEvaluate)
    {
        qreal distanceToConflict = calculateDistanceToConflict(conflict);
        if (distanceToConflict < 0) continue;

        qreal thisApproachTime = calculateTimeToReach(distanceToConflict, currentSpeed_, maxAcceleration_, cruiseSpeed_);

        for (const auto& other : conflict.priorityVehicles)
        {
            // Check if this vehicle can clear the conflict point well before the other vehicle arrives.
            if (!isSufficientlyAheadOf(thisApproachTime, other))
            {
                return false; // Unsafe to proceed, so stop checking and return.
            }
        }
    }

    return true; // No conflicts found, it's safe to proceed.
}
bool Vehicle::isSufficientlyAheadOf(qreal thisApproachTime, const PriorityVehicleInfo &other) const
{
    // If the conflict point is behind the other vehicle, we are safely ahead.
    if (other.distanceToConflictPoint < 0) return true;

    const qreal otherTimeToConflict = (other.vehicle->acceleration() > 0.2)
                                          ? calculateTimeToReach(
                                                other.distanceToConflictPoint,
                                                other.vehicle->speed(),
                                                other.vehicle->acceleration(),
                                                13.88) // TODO: Replace with a dynamic value, e.g., speed limit.
                                          : other.distanceToConflictPoint / other.vehicle->speed();

    // Check if our vehicle will clear the conflict point with a sufficient safety margin.
    return thisApproachTime + safetyTimeGap_ < otherTimeToConflict;
}

qreal Vehicle::calculateTimeToReach(qreal distance, qreal initialSpeed, qreal acceleration, qreal maxSpeed) const
{
    if (qAbs(acceleration) < 1e-6)
    {
        return distance / initialSpeed;
    }

    qreal distanceToAccelerate = (maxSpeed * maxSpeed - initialSpeed * initialSpeed) / (2 * acceleration);

    if (distanceToAccelerate >= distance)
    {
        // Vehicle reaches the point before or at the moment of achieving maxSpeed.
        // Solves for t from the kinematic equation: d = v₀t + ½at²
        // The positive root of the quadratic formula is used.
        qreal discriminant = initialSpeed * initialSpeed + 2 * acceleration * distance;
        return (-initialSpeed + std::sqrt(discriminant)) / acceleration;
    }
    else
    {
        // Vehicle accelerates to maxSpeed and then cruises at constant speed.
        qreal timeToAccelerate = (maxSpeed - initialSpeed) / acceleration;
        qreal distanceCruising = distance - distanceToAccelerate;
        qreal timeCruising = distanceCruising / maxSpeed;
        return timeToAccelerate + timeCruising;
    }
}

qreal Vehicle::calculateDistanceToConflict(const ConflictData &conflict) const
{
    if (currentTraversable_->type() == ITraversable::TraversableType::Lane)
    {
        auto* lane = static_cast<const Lane*>(currentTraversable_);
        const auto& nextTraversables = lane->next();
        if (nextTraversables.empty())
        {
            return std::numeric_limits<qreal>::max();
        }
        return lane->length(networkGeometry_) - progress_ +
               conflict.point->distanceFrom(static_cast<const Connection*>(nextTraversables[0]));
    }
    else // Assumes the type is Connection
    {
        auto* conn = static_cast<const Connection*>(currentTraversable_);
        return conflict.point->distanceFrom(conn) - progress_;
    }
}

void Vehicle::setNextDrivingBehavior(bool canProceed)
{
    if (canProceed)
    {
        behavior_ = DrivingBehavior::AdaptiveCruise;
        nextStopDistance_ = std::numeric_limits<qreal>::max();
    }
    else
    {
        behavior_ = DrivingBehavior::StopAtPoint;
        nextStopDistance_ = distanceToStopLine();
    }
}

VehiclePID::Input Vehicle::pidInput(qreal deltaTime)
{
    return {
        .action = behavior_,
        .currentSpeed = currentSpeed_,
        .deltaTime = deltaTime,
        .desiredCruiseSpeed = cruiseSpeed_,
        .distanceToStopPoint = nextStopDistance_,
        .maxAcceleration = maxAcceleration_,
        .comfortableDeceleration = comfortableDeceleration_,
        .maxDeceleration = maxDeceleration_,
        .safetyTimeGap = safetyTimeGap_,
        .safetyMinDistance = safetyMinDistance_
    };
}

qreal Vehicle::decisionDistance() const
{
    return (currentSpeed_ * currentSpeed_) / (2 * std::abs(comfortableDeceleration_));
}
