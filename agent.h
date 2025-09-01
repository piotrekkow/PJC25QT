#pragma once
#include <QPointF>
#include "navigationstrategy.h"

class Traversable;
class Traffic;
class GeometryManager;

class Agent
{
    std::unique_ptr<NavigationStrategy> navigationStrategy_;
    bool markedForRemoval_;

protected:
    const Traversable* traversable_;
    qreal progress_;    // progress along traversable
    qreal speed_;
    QPointF position_;
    qreal angle_;
    const qreal length_;
    const qreal width_;

    const Traffic* traffic_;
    const GeometryManager* geometry_;

    bool canProceed_;


public:
    virtual ~Agent() = default;

    Agent(const Traversable *traversable, const Traffic *traffic, const GeometryManager* geometry, qreal length, qreal width);
    void update(qreal deltaTime);

    bool isMarkedForRemoval() const { return markedForRemoval_; }
    qreal speed() const { return speed_; }
    qreal progress() const { return progress_; }
    QPointF position() const { return position_; }
    qreal angle() const { return angle_; }
    const Traversable* traversable() const { return traversable_; }
    qreal length() const { return length_; }
    qreal width() const { return width_; }

protected:
    virtual void updatePositionAndAngle();
    virtual void applyPhysics(qreal deltaTime);

    virtual std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) = 0;

private:
    void navigate();
};
