#include "agent.h"
#include "itraversable.h"
#include "geometrymanager.h"

Agent::Agent(const Traversable *traversable, const Traffic *traffic, const GeometryManager* geometry, qreal length, qreal width)
    : navigationStrategy_{ nullptr }
    , markedForRemoval_{ false }
    , traversable_{ traversable }
    , progress_{ 0.0 }
    , speed_{ 0.0 }
    , length_{ length }
    , width_{ width }
    , traffic_{ traffic }
    , geometry_{ geometry }
    , canProceed_{ false }
{}

void Agent::update(qreal deltaTime)
{
    navigate();
    updatePositionAndAngle();
    applyPhysics(deltaTime);
}

void Agent::updatePositionAndAngle()
{
    const QPainterPath* currentPath = &traversable_->path(geometry_);
    if (speed_ == 0 || !currentPath || currentPath->isEmpty()) return;

    qreal pathLength = currentPath->length();
    qreal clampedProgress = std::max(0.0, std::min((qreal)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}

void Agent::applyPhysics(qreal deltaTime)
{
    progress_ += speed_ * deltaTime;
}

void Agent::navigate()
{
    if (progress_ >= traversable_->length(geometry_))
    {
        traversable_ = navigationStrategy_->next();
        if (traversable_)
        {
            progress_ = 0;
            navigationStrategy_ = createNavigationStrategyFor(traversable_);
        }
        else
        {
            // End of route
            navigationStrategy_ = nullptr;
            markedForRemoval_ = true;
        }
    }

    navigationStrategy_->update();
}
