#pragma once

#include <QGraphicsObject>

class BaseObject : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit BaseObject(QGraphicsItem* parent = nullptr);
    virtual void showHandles() = 0;
    virtual void hideHandles() = 0;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void openPropertiesDialog(BaseObject* item);
};
