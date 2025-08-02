#pragma once

#include "renderer.h"
#include "roadnetwork.h"

class Simulation
{
    std::unique_ptr<RoadNetwork> network_;
    std::unique_ptr<Renderer> renderer_;

public:
    Simulation(QGraphicsScene* scene);

    void initialize();
    void update(float deltaTime);
};
