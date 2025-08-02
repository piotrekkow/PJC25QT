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
    auto is3 = network_->createIntersection({100,100});
    auto r1 = network_->createRoadway(is1, is2);
    auto r2 = network_->createRoadway(is2, is1);
    auto r3 = network_->createRoadway(is3, is1);

    // The Qt way to print debug info
    qDebug() << "Address of roadway opposite to r1 is" << (void*)r1->getOppositeRoadway();
    qDebug() << "Address of roadway opposite to r2 is" << (void*)r2->getOppositeRoadway();
    qDebug() << "Address of roadway opposite to r3 is" << (void*)r3->getOppositeRoadway();

    renderer_->draw();
}

void Simulation::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
}
