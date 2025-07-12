#include "roadtool.h"
#include "simulationscene.h"
#include "roaditem.h"
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
        // Create the final RoadItem instead of a QGraphicsLineItem
        if (rubberBandLine->line().length() > 10) {
            RoadItem *road = new RoadItem(rubberBandLine->line().p1(), rubberBandLine->line().p2());
            scene->addItem(road);
        }
        else
        {
            qDebug() << "Road segment too short";
        }

        // Remove the rubber-band line
        scene->removeItem(rubberBandLine);
        delete rubberBandLine;
        rubberBandLine = nullptr;
    }
}
