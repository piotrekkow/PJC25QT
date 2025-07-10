#pragma once

#include <QObject>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoom(QGraphicsView* view);

private:
    QGraphicsView* view_;
    float baseZoomFactor_;
    float zoomLevel_;
    QPointF targetScenePos_, targetViewportPos_;
    bool eventFilter(QObject* object, QEvent* event);

signals:
    void zoomed();

public slots:
    void resetZoom();
};
