#pragma once

#include <QObject>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoom(QGraphicsView* view);
    float getZoomLevel() const;

private:
    QGraphicsView* view_;
    float baseZoomFactor_;
    float zoomLevel_;
    QPointF targetScenePos_, targetViewportPos_;
    bool eventFilter(QObject* object, QEvent* event);

signals:
    void zoomed(float currentZoom);

public slots:
    void resetZoom();
};
