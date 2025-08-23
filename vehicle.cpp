#include "vehicle.h"
#include "lane.h"
#include "geometrymanager.h"

Vehicle::Vehicle(GeometryManager* networkGeometry, Lane* initialLane, qreal initialPosition)
    : networkGeometry_{ networkGeometry },
    currentTraversable_{static_cast<ITraversable*>(initialLane)},
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
            progress_ = currentTraversable_->length(networkGeometry_);
            currentSpeed_ = 0;
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
    qreal clampedProgress = std::max(0.0, std::min((double)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}
