#include "graphicsviewzoom.h"

#include <QMouseEvent>


GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
    : QObject{ view }
    , view_{ view }
    , baseZoomFactor_{ 1.004 }
    , zoomLevel_{ 1.0 }
{
    view_->viewport()->installEventFilter(this);
    // view_->setMouseTracking(true);

    view_->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view_->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

qreal GraphicsViewZoom::getZoomLevel() const
{
    return zoomLevel_;
}

bool GraphicsViewZoom::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        qreal angle = wheelEvent->angleDelta().y();
        qreal factor = qPow(baseZoomFactor_, angle);

        qreal futureLevel = zoomLevel_ * factor;

        if (futureLevel < 0.1 || futureLevel > 50.0) return true; // zoom limit

        zoomLevel_ = futureLevel;
        view_->scale(factor, factor);

        emit zoomed(zoomLevel_);
        return true;
    }

    Q_UNUSED(object)
    return false;
}

void GraphicsViewZoom::resetZoom()
{
    if (!view_ || !view_->scene())
    {
        return;
    }

    QRectF bounds = view_->scene()->itemsBoundingRect();

    if (bounds.isNull()) {
        return; // Scene is empty, nothing to do.
    }

    qreal padding = 50.0;
    bounds = bounds.marginsAdded(QMarginsF(padding, padding, padding, padding));
    view_->fitInView(bounds, Qt::KeepAspectRatio);

    zoomLevel_ = 1.0;
    emit zoomed(zoomLevel_);
}
