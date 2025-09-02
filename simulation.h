#pragma once

#include "renderer.h"
#include "roadnetwork.h"
#include "traffic.h"
#include <qdebug.h>

class Simulation
{
    const std::unique_ptr<RoadNetwork> network_;
    const std::unique_ptr<Traffic> traffic_;
    const std::unique_ptr<Renderer> renderer_;

public:
    Simulation(QGraphicsScene* scene);

    void initialize();
    void update(qreal deltaTime);
};
