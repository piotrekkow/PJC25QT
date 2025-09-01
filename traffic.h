#pragma once
// #include <qtypes.h>
// #include "vehicle.h"

// class RoadNetwork;
// class Lane;

// class Traffic
// {
//     std::vector<std::unique_ptr<Vehicle>> vehicles_;
//     const RoadNetwork* network_;

// public:
//     Traffic(const RoadNetwork* network);

//     void update(qreal deltaTime);

//     Vehicle* createVehicle(Lane* initialLane, qreal initialPosition = 0.0);
//     const std::vector<std::unique_ptr<Vehicle>>& vehicles() const { return vehicles_; }
//     void removeVehicle(Vehicle* vehicle);
// };

#include "agent.h"
#include "intersectioncontroller.h"
#include "intersectionrouter.h"
// #include "generator.h"
#include <memory>
#include <unordered_map>

class RoadNetwork;

class Traffic
{
    std::vector<std::unique_ptr<Agent>> agents_;
    // std::vector<std::unique_ptr<Generator>> generators_;
    // std::vector<std::unique_ptr<Router>> routers_;
    std::unordered_map<const Intersection*, std::unique_ptr<IntersectionController>> controllers_;
    std::unordered_map<const Intersection*, std::unique_ptr<IntersectionRouter>> routers_;

    RoadNetwork* network_;    // TODO: Make const, allow only changes only in an edit mode. Currently not const to allow construction of controllers and routers for intersections

public:
    /*explicit*/ Traffic(RoadNetwork* network);

    void update(qreal deltaTime);

    template<typename AgentType, typename... Args>
    AgentType* createAgent(Args&&... args)
    {
        agents_.push_back(AgentType::create(std::forward<Args>(args)...));
        return static_cast<AgentType*>(agents_.back().get());
    }

    const std::vector<std::unique_ptr<Agent>>& agents() const { return agents_; }
    void removeAgent(Agent* agent);

    Intersection* createIntersection(QPointF position);
    const IntersectionController* controller(const Intersection* intersection) const;
    const IntersectionRouter* router(const Intersection* intersection) const;
    IntersectionRouter* router(const Intersection* intersection);
    // template<typename GeneratorType, typename... Args>
    // void addGenerator(Args&&... args);
};
