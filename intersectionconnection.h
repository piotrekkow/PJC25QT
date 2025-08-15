#pragma once
#include <QLineF>

class Lane;

class IntersectionConnection
{
    Lane* sourceLane_;
    Lane* destinationLane_;
    // BezierCurve geometry_;  // point 2 of bezier curve at intersection of line extensions of corresponding lanes, points 1 and 3 adjustable to create the desired shape

public:
    IntersectionConnection(Lane* source, Lane* destination);
    Lane* source() const { return sourceLane_; }
    Lane* destination() const { return destinationLane_; }
};
