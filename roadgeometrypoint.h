#pragma once
#include <QPointF>

class RoadGeometryPoint
{
    QPointF position_;
    float medianWidth_;
    float lateralMedianOffset_;
public:
    RoadGeometryPoint(QPointF position, float medianWidth = 0.0f, float lateralMedianOffset = 0.0f);

    QPointF getPosition() const { return position_; }
    float getMedianWidth() const { return medianWidth_; }
    float getLateralMedianOffset() const { return lateralMedianOffset_; }

    void setPosition(QPointF& position) { position_ = position; }
    void setMedianWidth(float width) { medianWidth_ = width; }
    void setLateralMedianOffset_(float offset) { lateralMedianOffset_ = offset; }
};
