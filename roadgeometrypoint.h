#pragma once
#include <QPointF>

class RoadGeometryPoint
{
    QPointF position_;
    qreal medianWidth_;
    qreal medianOffset_;
public:
    RoadGeometryPoint(QPointF position, qreal medianWidth = 0.0, qreal medianOffset = 0.0);

    QPointF position() const { return position_; }
    qreal medianWidth() const { return medianWidth_; }
    qreal medianOffset() const { return medianOffset_; }

    void position(QPointF& position) { position_ = position; }
    void medianWidth(qreal width) { medianWidth_ = width; }
    void medianOffset(qreal offset) { medianOffset_ = offset; }
};
