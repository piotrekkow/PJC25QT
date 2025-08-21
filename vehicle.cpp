#include "vehicle.h"
#include "lane.h"
#include "geometrymanager.h"

Vehicle::Vehicle(GeometryManager* networkGeometry, Lane* initialLane, qreal initialPosition)
    : networkGeometry_{ networkGeometry },
    state_{DrivingState::ON_LANE},
    currentLane_{initialLane},
    progress_{initialPosition},
    currentSpeed_{0.0f},
    targetSpeed_{13.89f}, // ~50 km/h
    angle_{0.0f},
    color_{Qt::blue},
    length_{4.5f},
    width_{1.8f}
{
    updatePositionAndAngle();
}

void Vehicle::update(qreal deltaTime)
{
    // Simple acceleration model
    if (currentSpeed_ < targetSpeed_)
    {
        currentSpeed_ += 2.5f * deltaTime; // 2.5 m/s^2 acceleration
        currentSpeed_ = std::min(currentSpeed_, targetSpeed_);
    }

    progress_ += currentSpeed_ * deltaTime;

    if (state_ == DrivingState::ON_LANE)
    {
        const QPainterPath& lanePath = networkGeometry_->lane(currentLane_);
        const qreal laneLength = lanePath.length();

        if (progress_ >= laneLength)
        {
            const auto& connections = currentLane_->connections();
            if (!connections.empty())
            {
                // Transition to the connection
                currentConnection_ = connections[0]; // Take the first connection for simplicity
                state_ = DrivingState::ON_CONNECTION;
                progress_ -= laneLength; // Carry over the remaining progress
            }
            else
            {
                // No connection, stop at the end of the lane
                progress_ = laneLength;
                currentSpeed_ = 0;
            }
        }
    }
    else // state_ == DrivingState::ON_CONNECTION
    {
        const QPainterPath& connectionPath = networkGeometry_->connection(currentConnection_);
        const qreal connectionLength = connectionPath.length();

        if (progress_ >= connectionLength)
        {
            // Transition to the next lane
            currentLane_ = currentConnection_->destination();
            state_ = DrivingState::ON_LANE;
            progress_ -= connectionLength; // Carry over remaining progress
            currentConnection_ = nullptr;
        }
    }

    updatePositionAndAngle();
}

void Vehicle::updatePositionAndAngle()
{
    const QPainterPath* currentPath = nullptr;
    if (state_ == DrivingState::ON_LANE)
    {
        currentPath = &networkGeometry_->lane(currentLane_);
    }
    else // ON_CONNECTION
    {
        currentPath = &networkGeometry_->connection(currentConnection_);
    }

    if (!currentPath || currentPath->isEmpty()) return;

    qreal pathLength = currentPath->length();
    qreal clampedProgress = std::max(0.0, std::min((double)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}
