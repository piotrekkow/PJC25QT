#include "simulation.h"
#include <QDebug>

Simulation::Simulation(QGraphicsScene *scene)
    : network_{ std::make_unique<RoadNetwork>() }
    , traffic_{ std::make_unique<TrafficManager>(network_.get()) }
    , renderer_{ std::make_unique<Renderer>(scene, network_.get(), traffic_.get()) }
{
    initialize();
}

void Simulation::initialize()
{
    auto is1 = network_->createIntersection({0, 0});
    auto is2 = network_->createIntersection({0, 180});
    auto is3 = network_->createIntersection({190,-50});
    auto is4 = network_->createIntersection({-50, -100});
    auto is5 = network_->createIntersection({-200, 50});

    auto r2 = network_->createRoad(is1, is2);
    auto r3 = network_->createRoad(is1, is3);
    auto r4 = network_->createRoad(is1, is4);
    auto r5 = network_->createRoad(is1, is5);

    r2->createRoadways();
    r3->createRoadways();
    r4->createRoadway(is4);
    r5->createRoadways();

    r3->geometry().addPoint({30, -6}, 1);
    r3->geometry().pointAt(0).medianWidth(4.0f);
    r3->geometry().addPoint({50, -12}, 2);
    r3->geometry().pointAt(1).medianWidth(1.0f);
    r3->geometry().addPoint({100, -30}, 3);
    r3->geometry().addPoint({120, -42}, 4);
    r3->geometry().addPoint({140, -46}, 5);

    r2->backwardRoadway()->addLane();
    auto conn2130 = is1->createConnection(r2->backwardRoadway()->lanes()[1].get(), r3->forwardRoadway()->lanes()[0].get());
    conn2130->sourceOffset(2.0f);

    is1->createConnection(r2->backwardRoadway()->lanes()[0].get(), r4->forwardRoadway()->lanes()[0].get());

    auto conn3020 = is1->createConnection(r3->backwardRoadway()->lanes()[0].get(), r2->forwardRoadway()->lanes()[0].get());
    is1->createConnection(r3->backwardRoadway()->lanes()[0].get(), r4->forwardRoadway()->lanes()[0].get());
    conn3020->destinationOffset(3.0f);

    is1->createConnection(r5->backwardRoadway()->lanes()[0].get(), r3->forwardRoadway()->lanes()[0].get());
    is1->createConnection(r3->backwardRoadway()->lanes()[0].get(), r5->forwardRoadway()->lanes()[0].get());
    auto conn5020 = is1->createConnection(r5->backwardRoadway()->lanes()[0].get(), r2->forwardRoadway()->lanes()[0].get());
    conn5020->destinationOffset(1.0f);

    is1->createConnection(r2->backwardRoadway()->lanes()[0].get(), r5->forwardRoadway()->lanes()[0].get());

    r2->backwardRoadway()->priority(PriorityType::Priority);
    traffic_->createVehicle(r2->backwardRoadway()->lanes()[0].get());
    traffic_->createVehicle(r3->backwardRoadway()->lanes()[0].get());

    is1->conflictManager()->recalculate();
    renderer_->draw();
}

void Simulation::update(qreal deltaTime)
{
    traffic_->update(deltaTime);
    renderer_->draw();
}
