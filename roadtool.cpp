#include "roadtool.h"
#include "simulationscene.h"
#include <QPen>

RoadTool::RoadTool(QObject *parent)
    : Tool{parent}
    , rubberBandLine(nullptr)
{}

void RoadTool::mousePressEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->scenePos();
        rubberBandLine = new QGraphicsLineItem(QLineF(startPoint, startPoint));
        QPen pen(Qt::black, 2, Qt::DashLine);
        rubberBandLine->setPen(pen);
        scene->addItem(rubberBandLine);
    }
}

void RoadTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    if (rubberBandLine) {
        QLineF newLine(startPoint, event->scenePos());
        rubberBandLine->setLine(newLine);
    }
    Q_UNUSED(scene);
}

void RoadTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    if (rubberBandLine && event->button() == Qt::LeftButton) {
        // Create the final line item
        QGraphicsLineItem *line = new QGraphicsLineItem(rubberBandLine->line());
        line->setPen(QPen(Qt::black, 3));
        scene->addItem(line);

        // Remove the rubber-band line
        scene->removeItem(rubberBandLine);
        delete rubberBandLine;
        rubberBandLine = nullptr;
    }
}
