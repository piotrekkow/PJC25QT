#include "simulation.h"
#include "intersection.h"
#include "vehicle.h"
#include <QDebug>

Simulation::Simulation(QGraphicsScene *scene)
    : network_{ std::make_unique<RoadNetwork>() }
    , traffic_{ std::make_unique<Traffic>(network_.get()) }
    , renderer_{ std::make_unique<Renderer>(scene) }
{
    traffic_->addObserver(renderer_.get());
    initialize();
}

void Simulation::initialize()
{
    auto is1 = traffic_->createIntersection({0, 0});
    auto is2 = traffic_->createIntersection({0, 180});
    auto is3 = traffic_->createIntersection({190,-50});
    auto is4 = traffic_->createIntersection({-50, -100});
    auto is5 = traffic_->createIntersection({-200, 50});

    auto r2 = network_->createRoad(is1, is2);
    auto r3 = network_->createRoad(is1, is3);
    auto r4 = network_->createRoad(is1, is4);
    auto r5 = network_->createRoad(is1, is5);

    r2->createRoadways();
    r3->createRoadways();
    r4->createRoadway(is4);
    r5->createRoadways();

    r3->geometry().addPoint({30, -6}, 1);
    r3->geometry().pointAt(0).medianWidth(4.0);
    r3->geometry().addPoint({50, -12}, 2);
    r3->geometry().pointAt(1).medianWidth(1.0);
    r3->geometry().addPoint({100, -30}, 3);
    r3->geometry().addPoint({120, -42}, 4);
    r3->geometry().addPoint({140, -46}, 5);

    // r2->roadway(is1)->addLane();

    auto conn2130 = is1->createConnection(r2->roadway(is1)->lanes()[0].get(), r3->roadway(is3)->lanes()[0].get());
    conn2130->sourceOffset(2.0);

    is1->createConnection(r2->roadway(is1)->lanes()[0].get(), r4->roadway(is4)->lanes()[0].get());

    auto conn3020 = is1->createConnection(r3->roadway(is1)->lanes()[0].get(), r2->roadway(is2)->lanes()[0].get());
    conn3020->stopLineOffset(3.0);
    is1->createConnection(r3->roadway(is1)->lanes()[0].get(), r4->roadway(is4)->lanes()[0].get());
    conn3020->destinationOffset(3.0);

    is1->createConnection(r5->roadway(is1)->lanes()[0].get(), r3->roadway(is3)->lanes()[0].get());
    is1->createConnection(r3->roadway(is1)->lanes()[0].get(), r5->roadway(is5)->lanes()[0].get());
    auto conn5020 = is1->createConnection(r5->roadway(is1)->lanes()[0].get(), r2->roadway(is2)->lanes()[0].get());
    conn5020->destinationOffset(1.0);

    is1->createConnection(r2->roadway(is1)->lanes()[0].get(), r5->roadway(is5)->lanes()[0].get());

    // r2->roadway(is1)->priority(PriorityType::Priority);
    // r3->roadway(is1)->priority(PriorityType::Stop);

    auto is1Router = traffic_->router(is1);
    is1Router->addRoadwayFlows(r2->roadway(is1), {{r3->roadway(is3), 1}, {r4->roadway(is4), 1}, {r5->roadway(is5), 1}});
    is1Router->addRoadwayFlows(r3->roadway(is1), {{r2->roadway(is2), 1}, {r4->roadway(is4), 1}, {r5->roadway(is5), 1}});
    is1Router->addRoadwayFlows(r5->roadway(is1), {{r2->roadway(is2), 1}, {r3->roadway(is3), 1}});

    auto is2Gen = traffic_->generator(is2);
    is2Gen->flow(500.0);
    is2Gen->validate();

    auto is3Gen = traffic_->generator(is3);
    is3Gen->flow(300.0);
    is3Gen->validate();

    auto is5Gen = traffic_->generator(is5);
    is5Gen->flow(100.0);
    is5Gen->validate();

    traffic_->createAgent<Vehicle>(r2->roadway(is1)->lanes()[0].get(), traffic_.get(), network_->geometry());
    traffic_->createAgent<Vehicle>(r3->roadway(is1)->lanes()[0].get(), traffic_.get(), network_->geometry());

    is1->conflictManager()->recalculate();
    for (const auto& intersection : network_->intersections())
        traffic_->router(intersection.get())->validate();

    renderer_->initializeStaticLayer(network_.get());
}

void Simulation::update(qreal deltaTime)
{
    traffic_->update(deltaTime);
    renderer_->updateDynamicLayer(traffic_.get(), network_.get());
}
