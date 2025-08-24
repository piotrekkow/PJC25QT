#pragma once

#include <QGraphicsScene>

class RoadNetwork;
class TrafficManager;

class Renderer
{
    QGraphicsScene* scene_;
    const RoadNetwork* network_;
    const TrafficManager* traffic_;

public:
    Renderer(QGraphicsScene* scene, const RoadNetwork* network, const TrafficManager *traffic);
    void draw() const;

private:

    void drawArrow(QLineF baseline, qreal quiverLength, qreal quiverAngleDeg, QPen pen) const;
    void drawCircle(QPointF position, qreal diameter, QPen pen) const;

    QLineF offsetLine(const QLineF& line, qreal offset) const;
};
