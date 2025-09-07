#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QMouseEvent>

/**
 * @brief Adds graphics view panning functionality
 */
class GraphicsViewPannable : public QGraphicsView
{
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView; // inherit constructors from base

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool isPanning_ = false;
    QPoint lastMousePos_;
};
