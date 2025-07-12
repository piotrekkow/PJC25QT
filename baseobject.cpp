#include "baseobject.h"

BaseObject::BaseObject(QGraphicsItem *parent) : QGraphicsObject(parent) {
    // Make items selectable and movable
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
}

void BaseObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    // Emit a signal that the MainWindow can connect to
    emit openPropertiesDialog(this);
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void BaseObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
}
