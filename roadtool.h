#pragma once

#include "tool.h"
#include <QGraphicsLineItem>

class RoadTool : public Tool
{
    Q_OBJECT
public:
    explicit RoadTool(QObject *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) override;

private:
    QPointF startPoint;
    QGraphicsLineItem* rubberBandLine;
};
