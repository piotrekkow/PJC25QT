#include "objecthandle.h"
#include "roaditem.h"
#include <QBrush>
#include <QPen>

ObjectHandle::ObjectHandle(QGraphicsItem *parent) : QGraphicsRectItem(-5, -5, 10, 10, parent) {
    setBrush(Qt::green);

    // It's still good practice to be explicit about the pen
    setPen(QPen(Qt::black, 1));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIgnoresTransformations); // constant size with changing zoom level
}

QVariant ObjectHandle::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged && scene()) {
        RoadItem *road = qgraphicsitem_cast<RoadItem*>(parentItem());
        if (road) {
            road->handleMoved();
        }
    }
    return QGraphicsRectItem::itemChange(change, value);
}
