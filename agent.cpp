#include "agent.h"
#include "traversable.h"
#include "geometrymanager.h"
#include <QDebug>

Agent::Agent(const Traversable *traversable, const Traffic *traffic, const GeometryManager* geometry, qreal length, qreal width)
    : markedForRemoval_{ false }
    , traversable_{ traversable }
    , navigationStrategy_{ nullptr }
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
    if (navigationStrategy_)
    {
        updateDynamics(deltaTime);
    }

    applyPhysics(deltaTime);
    navigate();
}

void Agent::updatePositionAndAngle()
{
    if (!traversable_) return;
    const QPainterPath* currentPath = &traversable_->path(geometry_);
    if (!currentPath || currentPath->isEmpty()) return;

    qreal pathLength = currentPath->length();
    qreal clampedProgress = std::max(0.0, std::min((qreal)progress_, pathLength));
    qreal percent = currentPath->percentAtLength(clampedProgress);

    position_ = currentPath->pointAtPercent(percent);
    angle_ = currentPath->angleAtPercent(percent);
}

void Agent::applyPhysics(qreal deltaTime)
{
    progress_ += speed_ * deltaTime;
    updatePositionAndAngle();
}

void Agent::navigate()
{

    if (progress_ >= traversable_->length(geometry_))
    {
        if (!traversable_ || markedForRemoval_) return;

        const Traversable* nextTraversable = navigationStrategy_->next();
        if (nextTraversable)
        {
            traversable_ = nextTraversable;
            progress_ = 0;
            navigationStrategy_ = createNavigationStrategyFor(traversable_);
        }
        else
        {
            // End of route
            navigationStrategy_ = nullptr;
            markedForRemoval_ = true;

            qDebug() << this << "reached end of route." <<'\n';
        }
    }
}
