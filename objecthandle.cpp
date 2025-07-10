#include "objecthandle.h"
#include "roaditem.h" // The handle needs to know about its potential parent
#include <QBrush>

ObjectHandle::ObjectHandle(QGraphicsItem *parent) : QGraphicsRectItem(-5, -5, 10, 10, parent) {
    setBrush(Qt::green);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges); // Crucial for itemChange()
}

QVariant ObjectHandle::itemChange(GraphicsItemChange change, const QVariant &value) {
    // If the handle is being moved by the user...
    if (change == ItemPositionHasChanged && scene()) {
        // ...get the parent (the RoadItem) and tell it to update.
        RoadItem *road = qgraphicsitem_cast<RoadItem*>(parentItem());
        if (road) {
            road->handleMoved(); // Custom slot on the RoadItem
        }
    }
    return QGraphicsRectItem::itemChange(change, value);
}
