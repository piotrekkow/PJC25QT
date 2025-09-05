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

    auto r2 = network_->createRoad(is1, is2);   // bottom
    auto r3 = network_->createRoad(is1, is3);   // right
    auto r4 = network_->createRoad(is1, is4);   // top
    auto r5 = network_->createRoad(is1, is5);   // left

    r2->createRoadways();
    r3->createRoadways();
    r4->createRoadways();
    r5->createRoadways();

    r3->roadwayInto(is1)->priority(PriorityType::Priority);
    r5->roadwayInto(is1)->priority(PriorityType::Priority);
    r4->roadwayInto(is1)->priority(PriorityType::Stop);

    r3->geometry().addPoint({30, -6}, 1);
    r3->geometry().pointAt(0).medianWidth(4.0);
    r3->geometry().addPoint({50, -12}, 2);
    r3->geometry().pointAt(1).medianWidth(1.0);
    r3->geometry().addPoint({100, -30}, 3);
    r3->geometry().addPoint({120, -42}, 4);
    r3->geometry().addPoint({140, -46}, 5);

    is1->createConnection(r2, r3)->sourceOffset(2.0);
    is1->createConnection(r2, r4);
    auto c32 = is1->createConnection(r2, r5);
    is1->createConnection(r3, r2);
    is1->createConnection(r3, r4);
    is1->createConnection(r3, r5);
    is1->createConnection(r4, r2)->destinationOffset(1.0);
    is1->createConnection(r4, r3);
    is1->createConnection(r4, r5);
    is1->createConnection(r5, r2);
    is1->createConnection(r5, r3);
    is1->createConnection(r5, r4);

    c32->stopLineOffset(3.0);
    c32->destinationOffset(3.0);

    is1->conflictManager()->recalculate();

    // --- Phase 2: Construct and configure dynamic traffic objects

    traffic_ = std::make_unique<Traffic>(network_.get());
    traffic_->addObserver(renderer_.get());

    auto is1Router = traffic_->router(is1);
    is1Router->addRoadwayFlows(r2->roadwayInto(is1), {{r3->roadwayOutOf(is1), 1}, {r4->roadwayOutOf(is1), 1}, {r5->roadwayOutOf(is1), 1}});
    is1Router->addRoadwayFlows(r3->roadwayInto(is1), {{r2->roadwayOutOf(is1), 1}, {r4->roadwayOutOf(is1), 1}, {r5->roadwayOutOf(is1), 1}});
    is1Router->addRoadwayFlows(r4->roadwayInto(is1), {{r2->roadwayOutOf(is1), 1}, {r3->roadwayOutOf(is1), 1}, {r5->roadwayOutOf(is1), 1}});
    is1Router->addRoadwayFlows(r5->roadwayInto(is1), {{r2->roadwayOutOf(is1), 1}, {r3->roadwayOutOf(is1), 1}, {r4->roadwayOutOf(is1), 1}});

    traffic_->generator(is2)->flow(200.0);
    traffic_->generator(is3)->flow(400.0);
    traffic_->generator(is4)->flow(200.0);
    traffic_->generator(is5)->flow(600.0);

    traffic_->validate();

    // --- Phase 3: Initialize static topology in renderer

    renderer_->initializeStaticLayer(network_.get());
}

void Simulation::update(qreal deltaTime)
{
    traffic_->update(deltaTime);
    renderer_->updateDynamicLayer(traffic_.get(), network_.get());
}
