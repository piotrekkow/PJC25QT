#pragma once
#include <QPointF>

class RoadGeometryPoint
{
    QPointF position_;
    float medianWidth_;
    float medianOffset_;
public:
    RoadGeometryPoint(QPointF position, float medianWidth = 0.0f, float medianOffset = 0.0f);

    QPointF position() const { return position_; }
    float medianWidth() const { return medianWidth_; }
    float medianOffset() const { return medianOffset_; }

    void position(QPointF& position) { position_ = position; }
    void medianWidth(float width) { medianWidth_ = width; }
    void medianOffset(float offset) { medianOffset_ = offset; }

};
