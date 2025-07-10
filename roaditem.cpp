#include "roaditem.h"
#include <QPainter>

RoadItem::RoadItem(const QPointF &start, const QPointF &end, QGraphicsItem *parent)
    : BaseObject(parent), startPoint(start), endPoint(end) {

    // Create handles as children of this RoadItem
    startHandle = new ObjectHandle(this);
    endHandle = new ObjectHandle(this);

    // Position the handles
    startHandle->setPos(startPoint);
    endHandle->setPos(endPoint);

    // Hide them by default
    hideHandles();

    // The entire RoadItem itself is not movable; only its handles are.
    // We will move the object by code when a handle is dragged.
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

QRectF RoadItem::boundingRect() const {
    // Return a rectangle that encloses the line between the two points.
    // The QRectF constructor automatically handles which point is top-left vs bottom-right.
    return QRectF(startPoint, endPoint).normalized();
}

void RoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // You can customize the pen for color, width, style, etc.
    QPen roadPen(Qt::black, 5); // A thick black pen
    painter->setPen(roadPen);

    // Draw a line using the internal start and end points, which are
    // updated by the handleMoved() slot.
    painter->drawLine(startPoint, endPoint);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void RoadItem::showHandles() {
    startHandle->show();
    endHandle->show();
}

void RoadItem::hideHandles() {
    startHandle->hide();
    endHandle->hide();
}

void RoadItem::handleMoved() {
    // Update internal points based on handle positions
    prepareGeometryChange(); // Important call before changing the bounding rect
    startPoint = startHandle->pos();
    endPoint = endHandle->pos();
    update(); // Redraw the item
}
