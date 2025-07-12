#include "graphicsviewpan.h"
#include <QScrollBar>

void GraphicsViewPan::mousePressEvent(QMouseEvent* event)
{
    // Check if the middle mouse button was pressed
    if (event->button() == Qt::MiddleButton)
    {
        isPanning_ = true;
        // Record the original mouse position
        lastMousePos_ = event->pos();
        // Change the cursor to a closed hand to indicate panning
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    // For all other buttons, defer to the base class implementation
    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewPan::mouseMoveEvent(QMouseEvent* event)
{
    // Check if we are in panning mode
    if (isPanning_)
    {
        // Calculate the difference in position
        QPoint delta = event->pos() - lastMousePos_;
        lastMousePos_ = event->pos();

        // The key part: Move the scroll bars by the delta.
        // We use subtraction because dragging the mouse right (positive delta)
        // should move the content left (scroll bar value decreases).
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewPan::mouseReleaseEvent(QMouseEvent* event)
{
    // Check if the middle mouse button was released
    if (event->button() == Qt::MiddleButton)
    {
        isPanning_ = false;
        // Restore the default cursor
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }

    QGraphicsView::mouseReleaseEvent(event);
}
