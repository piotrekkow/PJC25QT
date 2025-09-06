#pragma once
#include <QPointF>
#include "navigationstrategy.h"

class Traversable;
class Traffic;
class GeometryManager;

class Agent
{
    bool markedForRemoval_; // reached end of route and is marked to be removed

protected:
    const Traversable* traversable_;    // topology an agent is currently traveling on
    std::unique_ptr<NavigationStrategy> navigationStrategy_;    // navigation rules of topology

    qreal progress_;        // progress along traversable in m
    qreal speed_;           // m/s
    QPointF position_;      // center of vehicle {m, m}
    qreal angle_;           // deg
    const qreal length_;    // m
    const qreal width_;     // m

    const Traffic* traffic_;
    const GeometryManager* geometry_;

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
    virtual void updateDynamics(qreal deltaTime) = 0;

    virtual std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) = 0;

private:
    void navigate();
};
