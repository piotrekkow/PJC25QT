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
};
