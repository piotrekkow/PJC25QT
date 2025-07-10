#pragma once

#include <QObject>
#include <QGraphicsSceneMouseEvent>

class SimulationScene;

class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject *parent = nullptr);
    virtual ~Tool() = default;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) = 0;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) = 0;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, SimulationScene* scene) = 0;

signals:
};
