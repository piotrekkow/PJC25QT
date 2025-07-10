#pragma once

#include "tool.h"
#include "baseobject.h"
#include <QGraphicsItem>

class SelectTool : public Tool
{
    Q_OBJECT

public:
    SelectTool(QObject* partent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) override;

private:
    QGraphicsItem *itemBeingMoved;
    BaseObject* currentlySelectedItem;
    QPointF originalPosition;
    QPointF pressPosition;
    bool isMoving;
};
