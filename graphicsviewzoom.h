#pragma once

#include <QObject>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoom(QGraphicsView* view);
    qreal getZoomLevel() const;

private:
    QGraphicsView* view_;
    qreal baseZoomFactor_;
    qreal zoomLevel_;
    QPointF targetScenePos_, targetViewportPos_;
    bool eventFilter(QObject* object, QEvent* event);

signals:
    void zoomed(qreal currentZoom);

public slots:
    void resetZoom();
};
