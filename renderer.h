#pragma once

#include <QGraphicsScene>

class RoadNetwork;
class Traffic;

class Renderer
{
    QGraphicsScene* scene_;
    const RoadNetwork* network_;
    const Traffic* traffic_;
    bool debug{ true };

public:
    Renderer(QGraphicsScene* scene, const RoadNetwork* network, const Traffic *traffic);
    void draw() const;

private:

    void drawArrow(QLineF baseline, qreal quiverLength, qreal quiverAngleDeg, QPen pen) const;
    void drawCircle(QPointF position, qreal diameter, QPen pen) const;

    QLineF offsetLine(const QLineF& line, qreal offset) const;
};
