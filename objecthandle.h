#pragma once

#include <QGraphicsRectItem>

class ObjectHandle : public QGraphicsRectItem
{
public:
    explicit ObjectHandle(QGraphicsItem *parent = nullptr);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};
