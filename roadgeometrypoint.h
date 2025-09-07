#pragma once
#include <QPointF>

class RoadGeometryPoint
{
    QPointF position_;
    qreal medianWidth_;
    qreal medianOffset_;
public:
    RoadGeometryPoint(QPointF position, qreal medianWidth = 0.0, qreal medianOffset = 0.0)
        : position_{ position }
        , medianWidth_{ medianWidth }
        , medianOffset_{ medianOffset }
    {}

    /**
     * @return position in {m, m}
     */
    QPointF position() const { return position_; }

    /**
     * @return width of the road median, centered at position()
     */
    qreal medianWidth() const { return medianWidth_; }

    /**
     * @return offset of the road median, positive to the right of road direction
     */
    qreal medianOffset() const { return medianOffset_; }

    /**
     * @brief setter for position in {m, m}
     */
    void position(QPointF& position) { position_ = position; }

    /**
     * @brief setter for width of the road median, centered at position()
     */
    void medianWidth(qreal width) { medianWidth_ = width; }

    /**
     * @brief setter for offset of the road median, positive to the right of road direction
     */
    void medianOffset(qreal offset) { medianOffset_ = offset; }
};
