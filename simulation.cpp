#include "simulation.h"


Simulation::Simulation(QGraphicsScene *scene)
{
    network_ = std::make_unique<RoadNetwork>();
    renderer_ = std::make_unique<Renderer>(scene, network_.get());
}

void Simulation::initialize()
{
    auto is1 = network_->createIntersection({0, 0});
    auto is2 = network_->createIntersection({0, 200});
    network_->createRoadway(is1, is2);
}

void Simulation::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
}
