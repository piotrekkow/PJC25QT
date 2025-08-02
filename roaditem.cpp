#include "roaditem.h"
#include <QPainter>
#include <QDebug> // For placeholder logging
#include <QGraphicsSceneMouseEvent> // For the event

const qreal ROAD_PEN_WIDTH = 16.0;

RoadItem::RoadItem(const QPointF &start, const QPointF &end, QGraphicsItem *parent)
    : BaseObject(parent), startPoint(start), endPoint(end), roadColor(Qt::gray) {

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
    qreal extra = ROAD_PEN_WIDTH;
    QRectF baseRect = QRectF(startPoint, endPoint)
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

    return baseRect.adjusted(0, 0, 1, 1);
}

void RoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // You can customize the pen for color, width, style, etc.
    QPen roadPen(roadColor, ROAD_PEN_WIDTH);
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
    update();
    // Update internal points based on handle positions
    prepareGeometryChange(); // Important call before changing the bounding rect
    startPoint = startHandle->pos();
    endPoint = endHandle->pos();
}

void RoadItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "RoadItem double-clicked!";
    // TODO: Create and show your properties dialog here.
    event->accept(); // Accept the event to stop it from propagating further
}

// void RoadItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
//     qDebug() << "Road pressed";
//     roadColor = Qt::lightGray;
//     event->accept();
// }
