#pragma once

#include <QPointF>
#include <QColor>

class Lane; // Forward declaration to avoid circular dependencies
class IntersectionConnection;
class GeometryManager;

enum class DrivingState {
    ON_LANE,
    ON_CONNECTION
};

class Vehicle
{
public:
    Vehicle(GeometryManager* networkGeometry, Lane* initialLane, float initialPosition = 0.0f);

    void update(float deltaTime);

    // --- Getters ---
    QPointF position() const { return position_; }
    float angle() const { return angle_; }
    QColor color() const { return color_; }
    float length() const { return length_; }
    float width() const { return width_; }

private:
    void updatePositionAndAngle();

    GeometryManager* networkGeometry_;
    DrivingState state_;
    Lane* currentLane_;
    IntersectionConnection* currentConnection_;
    float progress_; // Distance from the start of the lane in meters

    float currentSpeed_; // in m/s
    float targetSpeed_;  // in m/s

    QPointF position_;
    float angle_;      // in degrees

    // --- Vehicle Properties ---
    QColor color_;
    float length_;
    float width_;
};
