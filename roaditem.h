#pragma once

#include "baseobject.h"
#include "objecthandle.h"

class RoadItem : public BaseObject
{
    Q_OBJECT
public:
    explicit RoadItem(const QPointF &start, const QPointF &end, QGraphicsItem *parent = nullptr);

    // Required by QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Implementation of the BaseObject interface
    void showHandles() override;
    void hideHandles() override;

public slots:
    // Slot to react to handle movement
    void handleMoved();

private:
    ObjectHandle *startHandle;
    ObjectHandle *endHandle;
    QPointF startPoint;
    QPointF endPoint;
    QColor roadColor;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};
