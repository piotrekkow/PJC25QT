#pragma once
#include <QPointF>
#include "navigationstrategy.h"

class Traversable;
class Traffic;
class GeometryManager;

/**
 * @brief Agents are dynamic autonomous entities
 */
class Agent
{
    bool markedForRemoval_; // reached end of route and is marked to be removed

protected:
    const Traversable* traversable_;    //
    std::unique_ptr<NavigationStrategy> navigationStrategy_;    // navigation rules of topology

    qreal progress_;
    qreal speed_;
    QPointF position_;      // center of vehicle {m, m}
    qreal angle_;           // deg
    const qreal length_;    // m
    const qreal width_;     // m

    const Traffic* traffic_;
    const GeometryManager* geometry_;

public:
    virtual ~Agent() = default;

    Agent(const Traversable *traversable, const Traffic *traffic, const GeometryManager* geometry, qreal length, qreal width);

    /**
     * @brief updates agent's state based on time delta
     * @param deltaTime
     */
    void update(qreal deltaTime);

    bool isMarkedForRemoval() const { return markedForRemoval_; }

    /**
     * @return speed in m/s
     */
    qreal speed() const { return speed_; }

    /**
     * @return progress along traversable in m
     */
    qreal progress() const { return progress_; }

    /**
     * @return agent's centerpoint in {m, m}
     */
    QPointF position() const { return position_; }

    /**
     * @return in degrees
     */
    qreal angle() const { return angle_; }

    /**
     * @return topology an agent is currently traveling on
     */
    const Traversable* traversable() const { return traversable_; }

    /**
     * @return in m
     */
    qreal length() const { return length_; }

    /**
     * @return in m
     */
    qreal width() const { return width_; }

protected:
    virtual void updatePositionAndAngle();
    virtual void applyPhysics(qreal deltaTime);
    virtual void updateDynamics(qreal deltaTime) = 0;

    virtual std::unique_ptr<NavigationStrategy> createNavigationStrategyFor(const Traversable* newTraversable) = 0;

private:
    void navigate();
};
