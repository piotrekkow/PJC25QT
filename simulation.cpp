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

    auto is21 = network_->createIntersection({100, 260});

    auto is522 = network_->createIntersection({-220, 220}); // lower left corner

    auto is51 = network_->createIntersection({-250, -50});
    auto is53 = network_->createIntersection({-400, 60});

    auto is5221 = network_->createIntersection({-430, 200});
    auto is5222 = network_->createIntersection({-200, 400});

    auto r2 = network_->createRoad(is1, is2);   // bottom
    auto r3 = network_->createRoad(is1, is3);   // right
    auto r4 = network_->createRoad(is1, is4);   // top
    network_->createRoad(is1, is5);   // left

    auto r221 = network_->createRoad(is2, is21);
    auto r2522 = network_->createRoad(is2, is522);

    network_->createRoad(is5, is51);
    auto r522 = network_->createRoad(is5, is522);
    network_->createRoad(is5, is53);

    network_->createRoad(is522, is5221);
    auto r5222 = network_->createRoad(is522, is5222);

    r4->roadwayInto(is1)->priority(PriorityType::Priority);
    r2->roadwayInto(is1)->priority(PriorityType::Priority);
    r3->roadwayInto(is1)->priority(PriorityType::Stop);

    r522->roadwayInto(is522)->priority(PriorityType::Priority);
    r5222->roadwayInto(is522)->priority(PriorityType::Priority);

    r2->roadwayInto(is2)->priority(PriorityType::Priority);
    r2522->roadwayInto(is2)->priority(PriorityType::Priority);
    r221->roadwayInto(is2)->priority(PriorityType::Stop);

    r3->geometry().addPoint({30, -6}, 1);
    r3->geometry().pointAt(0).medianWidth(4.0);
    r3->geometry().addPoint({50, -12}, 2);
    r3->geometry().pointAt(1).medianWidth(1.0);
    r3->geometry().addPoint({100, -30}, 3);
    r3->geometry().addPoint({120, -42}, 4);
    r3->geometry().addPoint({140, -46}, 5);

    is1->createAllConnections();
    is5->createAllConnections();
    is2->createAllConnections();
    is522->createAllConnections();

    is1->conflictManager()->recalculate();

    // --- Phase 2: Construct and configure dynamic traffic objects

    traffic_ = std::make_unique<Traffic>(network_.get());
    traffic_->addObserver(renderer_.get());

    traffic_->router(is1)->addAllRoadwayFlows();
    traffic_->router(is2)->addAllRoadwayFlows();
    traffic_->router(is5)->addAllRoadwayFlows();
    traffic_->router(is522)->addAllRoadwayFlows();

    traffic_->generator(is21)->flow(200.0);
    traffic_->generator(is5221)->flow(150.0);
    traffic_->generator(is3)->flow(200.0);
    traffic_->generator(is4)->flow(150.0);
    traffic_->generator(is51)->flow(200.0);
    traffic_->generator(is5222)->flow(100.0);
    traffic_->generator(is53)->flow(200.0);

    traffic_->validate();

    // --- Phase 3: Initialize static topology in renderer

    renderer_->initializeStaticLayer(network_.get());
}

void Simulation::update(qreal deltaTime)
{
    traffic_->update(deltaTime);
    renderer_->updateDynamicLayer(traffic_.get(), network_.get());
}
