// #include "simulationview.h"
// #include <QScrollBar>

// SimulationView::SimulationView(QWidget* parent)
//     : QGraphicsView(parent), panning_{false}
// {
//     setRenderHint(QPainter::Antialiasing);
//     setDragMode(QGraphicsView::NoDrag);
//     setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
// }

// void SimulationView::wheelEvent(QWheelEvent* event)
// {
//     if (event->angleDelta().y() > 0)
//     {
//         scale(1.25, 1.25);
//     }
//     else
//     {
//         scale(0.8, 0.8);
//     }
// }

// void SimulationView::mousePressEvent(QMouseEvent* event)
// {
//     if (event->button() == Qt::RightButton)
//     {
//         panning_ = true;
//         lastPanPoint_ = event->pos();
//         setCursor(Qt::ClosedHandCursor);
//     }
//     QGraphicsView::mousePressEvent(event);
// }

// void SimulationView::mouseMoveEvent(QMouseEvent* event)
// {
//     if (panning_)
//     {
//         QPoint delta = event->pos() - lastPanPoint_;
//         horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
//         verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
//         lastPanPoint_ = event->pos();
//     }
//     QGraphicsView::mouseMoveEvent(event);
// }

// void SimulationView::mouseReleaseEvent(QMouseEvent* event)
// {
//     if (event->button() == Qt::RightButton)
//     {
//         panning_ = false;
//         setCursor(Qt::ArrowCursor);
//     }
//     QGraphicsView::mouseReleaseEvent(event);
// }

