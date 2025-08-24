#pragma once

#include <QPointF>
#include <QColor>

class Lane; // Forward declaration to avoid circular dependencies
class Connection;
class GeometryManager;
class ITraversable;

class Vehicle
{
public:
    Vehicle(const GeometryManager* networkGeometry, Lane* initialLane, qreal initialPosition = 0.0);

    void update(qreal deltaTime);

    // --- Getters ---
    QPointF position() const { return position_; }
    qreal angle() const { return angle_; }
    QColor color() const { return color_; }
    qreal length() const { return length_; }
    qreal width() const { return width_; }
    qreal progress() const { return progress_; }
    ITraversable* traversable() const { return currentTraversable_; }
    bool hasReachedDestination() const { return hasReachedDestination_; }

private:
    void updatePositionAndAngle();

    const GeometryManager* networkGeometry_;
    ITraversable* currentTraversable_;
    qreal progress_; // Distance from the start of the lane in meters
    bool hasReachedDestination_;

    qreal currentSpeed_; // in m/s
    qreal targetSpeed_;  // in m/s

    QPointF position_;
    qreal angle_;      // in degrees

    // --- Vehicle Properties ---
    QColor color_;
    qreal length_;
    qreal width_;
};
