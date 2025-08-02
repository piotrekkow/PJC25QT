#pragma once

#include <QGraphicsScene>

class RoadNetwork;

class Renderer
{
public:
    Renderer(QGraphicsScene* scene, RoadNetwork* network);
    void draw();

private:
    QGraphicsScene* scene_;
    RoadNetwork* network_;

    void drawArrow(QLineF baseline, qreal quiverLength, qreal quiverAngleDeg, QPen pen);
    QLineF offsetLine(const QLineF& line, qreal offset);
};
