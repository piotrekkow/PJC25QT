#pragma once

#include <QPointF>
#include <QColor>

class Lane; // Forward declaration to avoid circular dependencies
class Connection;
class GeometryManager;

enum class DrivingState {
    ON_LANE,
    ON_CONNECTION
};

class Vehicle
{
public:
    Vehicle(GeometryManager* networkGeometry, Lane* initialLane, qreal initialPosition = 0.0f);

    void update(qreal deltaTime);

    // --- Getters ---
    QPointF position() const { return position_; }
    qreal angle() const { return angle_; }
    QColor color() const { return color_; }
    qreal length() const { return length_; }
    qreal width() const { return width_; }

private:
    void updatePositionAndAngle();

    GeometryManager* networkGeometry_;
    DrivingState state_;
    Lane* currentLane_;
    Connection* currentConnection_;
    qreal progress_; // Distance from the start of the lane in meters

    qreal currentSpeed_; // in m/s
    qreal targetSpeed_;  // in m/s

    QPointF position_;
    qreal angle_;      // in degrees

    // --- Vehicle Properties ---
    QColor color_;
    qreal length_;
    qreal width_;
};
