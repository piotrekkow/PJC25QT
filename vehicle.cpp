#include "vehicle.h"
#include "IntersectionDecisionData.h"
#include "lane.h"
#include "geometrymanager.h"
#include "intersection.h"
#include "trafficmanager.h"

Vehicle::Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, TrafficManager* trafficManager, qreal initialPosition)
    : networkGeometry_{ networkGeometry }
    , currentTraversable_{ static_cast<ITraversable*>(initialLane) }
    , progress_{ initialPosition }
    , hasReachedDestination_{ false }
    , currentSpeed_{ 0.0 } // ~50 km/h
    , targetSpeed_{ 13.89}
    , currentAcceleration_{ 0.0 }
    , angle_{ 0.0 }
    , color_{ Qt::blue }
    , length_{ 4.5 }
    , width_{ 1.8 }
    , maxAcceleration_{ 2.5 }
    , comfortableDeceleration_{ -2.0 }
    , maxDeceleration_{ -5.0 }
    , state_{ VehicleState::Proceeding }
    , trafficManager_{ trafficManager }
{
    updatePositionAndAngle();
}

void Vehicle::update(qreal deltaTime)
{
    if (distanceToStopLine() <= decisionDistance() && state_ != VehicleState::Stopping)
        updateDecision();

    if (state_ == VehicleState::Proceeding)
    {
        currentAcceleration_ = maxAcceleration_;
    }

    if (state_ == VehicleState::Stopping)
    {
        stopping(distanceToStopLine());
    }

    if (state_ == VehicleState::Waiting)
    {
        currentAcceleration_ = 0.0;
        currentSpeed_ = 0.0;
    }
    else
        applyPhysics(deltaTime);

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

    updatePositionAndAngle();
}

void Vehicle::updatePositionAndAngle()
{
    const QPainterPath* currentPath = &currentTraversable_->path(networkGeometry_);
    if (!currentPath || currentPath->isEmpty()) return;

    qreal pathLength = currentPath->length();
    qreal clampedProgress = std::max(0.0, std::min((qreal)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}

void Vehicle::stopping(qreal distanceToStopPoint)
{
    if (distanceToStopPoint <= comfortableDeceleration_ && distanceToStopPoint > 0.1)
    {
        qreal requiredDeceleration = currentSpeed_ * currentSpeed_ / (2 * comfortableDeceleration_);
        currentAcceleration_ = std::max(requiredDeceleration, maxDeceleration_);
    }
    else if (currentSpeed_ > 0 && distanceToStopPoint <= 0.1)
    {
        currentAcceleration_ = maxDeceleration_;
    }
}

qreal Vehicle::distanceToStopLine()
{
    if (currentTraversable_->type() == ITraversable::TraversableType::Connection)
    {
        auto* conn = static_cast<const Connection*>(currentTraversable_);

        if (conn->stopLineOffset() - progress_ < 0)
        {
            qreal nextLaneLength = conn->next()[0]->length(networkGeometry_);
            qreal remainderOnThisConnection = conn->length(networkGeometry_) - progress_;
            qreal nextConnectionStopOffset = static_cast<const Connection*>(conn->next()[0]->next()[0])->stopLineOffset();
            return nextLaneLength + remainderOnThisConnection + nextConnectionStopOffset;
        }
        return conn->stopLineOffset() - progress_;
    }
    else
    {
        qreal remainingLaneLength = currentTraversable_->length(networkGeometry_) - progress_;
        qreal nextConnectionStopOffset = static_cast<const Connection*>(currentTraversable_->next()[0])->stopLineOffset();
        return remainingLaneLength + nextConnectionStopOffset;
    }
}

void Vehicle::applyPhysics(qreal deltaTime)
{
    currentSpeed_ += currentAcceleration_ * deltaTime;
    currentSpeed_ = std::max(0.0, currentSpeed_);
    progress_ += currentSpeed_ * deltaTime;
}

const qreal PERCIEVED_SAFETY_MARGIN{ 1.5 }; // s
void Vehicle::updateDecision()
{
    const IntersectionDecisionData decisionData = currentTraversable_->intersection()->controller()->decisionData(this, trafficManager_->vehicles());
    if (decisionData.mustPerformFullStop == true) state_ = VehicleState::Stopping;

    for (const auto& conflict : decisionData.conflictsToEvaluate)
    {
        qreal distanceToConflict;
        if (currentTraversable_->type() == ITraversable::TraversableType::Lane)
        {
            auto* lane = static_cast<const Lane*>(currentTraversable_);
            distanceToConflict = lane->length(networkGeometry_) - progress_ + conflict.point->distanceFrom(static_cast<const Connection*>(lane->next()[0]));
        }
        else
        {
            auto* conn = static_cast<const Connection*>(currentTraversable_);
            distanceToConflict = conflict.point->distanceFrom(conn) - progress_;
        }
        qreal thisApproachTime;
        qreal v_cap = 13.88; // m/s

        qreal s_accelThis = (v_cap * v_cap - currentSpeed_ * currentSpeed_) / (2 * maxAcceleration_);   // distance covered by trying to reach max speed by constant acceleration
        if (s_accelThis > distanceToConflict)
            thisApproachTime = std::max((-currentSpeed_ + std::sqrt(currentSpeed_ * currentSpeed_ + 2 * maxAcceleration_ * distanceToConflict)) / maxAcceleration_,
                                        (-currentSpeed_ - std::sqrt(currentSpeed_ * currentSpeed_ + 2 * maxAcceleration_ * distanceToConflict)) / maxAcceleration_);
        else
            thisApproachTime = (v_cap - currentSpeed_) / maxAcceleration_ + (distanceToConflict - s_accelThis) / v_cap;


        for (const auto& other : conflict.priorityVehicles)
        {
            qreal basicApproachTime = other.distanceToConflictPoint / other.vehicle->speed();
            qreal accelAdjustedApproachTime;
            if (other.vehicle->acceleration() > 0.1)
            {
                qreal t_accel = (v_cap - other.vehicle->speed()) / other.vehicle->acceleration();
                qreal s_accel = (v_cap * v_cap - other.vehicle->speed() * other.vehicle->speed()) / (2 * other.vehicle->acceleration());
                if (s_accel > other.distanceToConflictPoint)
                {
                    accelAdjustedApproachTime = std::max((-other.vehicle->speed() + std::sqrt(other.vehicle->speed() * other.vehicle->speed() + 2 * other.vehicle->acceleration() * distanceToConflict)) / other.vehicle->acceleration(),
                                                         (-other.vehicle->speed() - std::sqrt(other.vehicle->speed() * other.vehicle->speed() + 2 * other.vehicle->acceleration() * distanceToConflict)) / other.vehicle->acceleration());
                }
                accelAdjustedApproachTime = t_accel + (other.distanceToConflictPoint - s_accel) / v_cap;
            }

            if (thisApproachTime + PERCIEVED_SAFETY_MARGIN < basicApproachTime &&
                thisApproachTime + PERCIEVED_SAFETY_MARGIN < accelAdjustedApproachTime)
                state_ = VehicleState::Proceeding;
            else if (currentSpeed_ > 0)
                state_ = VehicleState::Yielding;
            else
                state_ = VehicleState::Waiting;
        }
    }
}
