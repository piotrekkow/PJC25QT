#include "simulation.h"


Simulation::Simulation(QGraphicsScene *scene)
{
    network_ = std::make_unique<RoadNetwork>();
    renderer_ = std::make_unique<Renderer>(scene, network_.get());
    initialize();
}

void Simulation::initialize()
{
    auto is1 = network_->createIntersection({0, 0});
    auto is2 = network_->createIntersection({0, 200});
    network_->createRoadway(is1, is2);
    network_->createRoadway(is2, is1);
    renderer_->draw();
}

void Simulation::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
}
