#pragma once

#include "agent.h"
#include "flowcontroller.h"
#include "flowrouter.h"
#include "flowgenerator.h"
#include "trafficobserver.h"
#include <memory>
#include <unordered_map>

class RoadNetwork;

class Traffic
{
    std::vector<std::unique_ptr<Agent>> agents_;
    std::unordered_map<const Intersection*, std::unique_ptr<FlowController>> controllers_;
    std::unordered_map<const Intersection*, std::unique_ptr<FlowRouter>> routers_;
    std::unordered_map<const Intersection*, std::unique_ptr<FlowGenerator>> generators_;
    std::unordered_map<const Intersection*, int> removedVehicleCounts_;

    std::vector<TrafficObserver*> observers_;

    // TODO: Make const, allow changes only in an `edit mode`. Currently not const to allow construction of controllers and routers for intersections
    RoadNetwork* network_;

public:
    Traffic(RoadNetwork* network);

    void update(qreal deltaTime);
    void validate();

    void addObserver(TrafficObserver* observer) { observers_.push_back(observer); }
    void removeObserver(TrafficObserver* observer) {observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end()); }

    template<typename AgentType, typename... Args>
    AgentType* createAgent(Args&&... args)
    {
        agents_.push_back(AgentType::create(std::forward<Args>(args)...));
        AgentType* newAgent = static_cast<AgentType*>(agents_.back().get());

        signalAgentAdded(newAgent);

        return newAgent;
    }

    const std::vector<std::unique_ptr<Agent>>& agents() const { return agents_; }
    void removeAgent(Agent* agent);

    // Intersection* createIntersection(QPointF position);

    const FlowController* controller(const Intersection* intersection) const;
    const FlowRouter* router(const Intersection* intersection) const;
    FlowRouter* router(const Intersection* intersection);
    const FlowGenerator *generator(const Intersection *intersection) const;
    FlowGenerator* generator(const Intersection* intersection);
    int removedVehicleCount(const Intersection* intersection) const;

    // template<typename GeneratorType, typename... Args>
    // void addGenerator(Args&&... args);

    const Vehicle* findLeadVehicle(const Agent* agent) const;

private:
    void populateIntersection(Intersection* intersection);
    void signalAgentAdded(const Agent* agent);
    void signalAgentRemoved(const Agent* agent);
};
