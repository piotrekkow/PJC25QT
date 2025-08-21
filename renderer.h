#pragma once

#include <QGraphicsScene>

class RoadNetwork;
class Road;

class Renderer
{
    QGraphicsScene* scene_;
    const RoadNetwork* network_;

public:
    Renderer(QGraphicsScene* scene, const RoadNetwork* network);
    void draw() const;

private:

    void drawArrow(QLineF baseline, qreal quiverLength, qreal quiverAngleDeg, QPen pen) const;
    void drawCircle(QPointF position, qreal diameter, QPen pen) const;

    QLineF offsetLine(const QLineF& line, qreal offset) const;
};
