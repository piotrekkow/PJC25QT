#include "simulation.h"
#include "intersection.h"
#include "vehicle.h"
#include <QDebug>

Simulation::Simulation(QGraphicsScene *scene)
    : network_{ std::make_unique<RoadNetwork>() }
    , renderer_{ std::make_unique<Renderer>(scene) }
{
    initialize();
}

void Simulation::initialize()
{
    // --- Phase 1: Configure static network topology ---

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
    r3->geometry().pointAt(0).medianWidth(4.0);
    r3->geometry().addPoint({50, -12}, 2);
    r3->geometry().pointAt(1).medianWidth(1.0);
    r3->geometry().addPoint({100, -30}, 3);
    r3->geometry().addPoint({120, -42}, 4);
    r3->geometry().addPoint({140, -46}, 5);

    // r2->roadway(is1)->addLane();

    auto conn2130 = is1->createConnection(r2->roadwayInto(is1)->lanes()[0].get(), r3->roadwayInto(is3)->lanes()[0].get());
    conn2130->sourceOffset(2.0);

    is1->createConnection(r2->roadwayInto(is1)->lanes()[0].get(), r4->roadwayInto(is4)->lanes()[0].get());

    auto conn3020 = is1->createConnection(r3->roadwayInto(is1)->lanes()[0].get(), r2->roadwayInto(is2)->lanes()[0].get());
    conn3020->stopLineOffset(3.0);
    is1->createConnection(r3->roadwayInto(is1)->lanes()[0].get(), r4->roadwayInto(is4)->lanes()[0].get());
    conn3020->destinationOffset(3.0);

    is1->createConnection(r5->roadwayInto(is1)->lanes()[0].get(), r3->roadwayInto(is3)->lanes()[0].get());
    is1->createConnection(r3->roadwayInto(is1)->lanes()[0].get(), r5->roadwayInto(is5)->lanes()[0].get());
    auto conn5020 = is1->createConnection(r5->roadwayInto(is1)->lanes()[0].get(), r2->roadwayInto(is2)->lanes()[0].get());
    conn5020->destinationOffset(1.0);

    is1->createConnection(r2->roadwayInto(is1)->lanes()[0].get(), r5->roadwayInto(is5)->lanes()[0].get());

    is1->conflictManager()->recalculate();

    qDebug() << "Topology init complete.";

    // --- Phase 2: Construct and configure dynamic traffic objects

    traffic_ = std::make_unique<Traffic>(network_.get());
    traffic_->addObserver(renderer_.get());

    // r2->roadway(is1)->priority(PriorityType::Priority);
    // r3->roadway(is1)->priority(PriorityType::Stop);

    qDebug() << "Traffic construction succesful.";

    auto is1Router = traffic_->router(is1);

    is1Router->addRoadwayFlows(r2->roadwayInto(is1), {{r3->roadwayInto(is3), 1}, {r4->roadwayInto(is4), 1}, {r5->roadwayInto(is5), 1}});
    is1Router->addRoadwayFlows(r3->roadwayInto(is1), {{r2->roadwayInto(is2), 1}, {r4->roadwayInto(is4), 1}, {r5->roadwayInto(is5), 1}});
    is1Router->addRoadwayFlows(r5->roadwayInto(is1), {{r2->roadwayInto(is2), 1}, {r3->roadwayInto(is3), 1}});
    (is1Router) ? qDebug() << "router exists."
                : qDebug() << "router missing.";

    auto is2Gen = traffic_->generator(is2);
    is2Gen->flow(1200.0);

    auto is3Gen = traffic_->generator(is3);
    is3Gen->flow(1300.0);

    auto is5Gen = traffic_->generator(is5);
    is5Gen->flow(1500.0);

    // traffic_->createAgent<Vehicle>(r2->roadway(is1)->lanes()[0].get(), traffic_.get(), network_->geometry());
    // traffic_->createAgent<Vehicle>(r3->roadway(is1)->lanes()[0].get(), traffic_.get(), network_->geometry());

    traffic_->validate();

    qDebug() << "Trafic init complete.";

    // --- Phase 3: Initialize static topology in renderer

    renderer_->initializeStaticLayer(network_.get());

    qDebug() << "Renderer init complete.";
}

void Simulation::update(qreal deltaTime)
{
    traffic_->update(deltaTime);
    renderer_->updateDynamicLayer(traffic_.get(), network_.get());
}
