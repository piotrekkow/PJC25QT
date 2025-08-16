#include "simulation.h"
#include <QDebug>

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
    auto is3 = network_->createIntersection({200,50});
    auto is4 = network_->createIntersection({-50, -200});
    auto r2 = network_->createRoad(is1, is2);
    auto r3 = network_->createRoad(is1, is3);
    auto r4 = network_->createRoad(is1, is4);

    r2->createRoadways();
    r3->createRoadways();
    r4->createRoadway(is4);

    r2->backwardRoadway()->addLane();
    r2->backwardRoadway()->lanes()[1].get()->addConnection(r3->forwardRoadway()->lanes()[0].get());
    r2->backwardRoadway()->lanes()[0].get()->addConnection(r4->forwardRoadway()->lanes()[0].get());
    r3->backwardRoadway()->lanes()[0].get()->addConnection(r2->forwardRoadway()->lanes()[0].get());
    renderer_->draw();
}

void Simulation::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
}
