#pragma once

#include "agent.h"
#include "flowcontroller.h"
#include "flowrouter.h"
#include "flowgenerator.h"
#include "trafficobserver.h"
#include <memory>
#include <unordered_map>

class RoadNetwork;

/**
 * @brief The dynamic layer of the simulation.
 */
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

    /**
     * @brief validates whether all relevant dynamic objects have been assigned valid values
     */
    void validate();

    /**
     * @brief adds observer, observers are objects have to know whether an object is about to be deleted and enact relevant cleanup before traffic removes that object (for agents)
     */
    void addObserver(TrafficObserver* observer) { observers_.push_back(observer); }

    /**
     * @brief removes an observer
     */
    void removeObserver(TrafficObserver* observer) {observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end()); }

    /**
     * @brief creates agent of given type
     * @param args differ depending on agent type
     * @return pointer to created agent
     */
    template<typename AgentType, typename... Args>
    AgentType* createAgent(Args&&... args)
    {
        agents_.push_back(AgentType::create(std::forward<Args>(args)...));
        AgentType* newAgent = static_cast<AgentType*>(agents_.back().get());

        signalAgentAdded(newAgent);

        return newAgent;
    }

    /**
     * @return reference to vector of all agents currently in simulation
     */
    const std::vector<std::unique_ptr<Agent>>& agents() const { return agents_; }

    /**
     * @brief removes an agent
     * @param agent to be removed
     */
    void removeAgent(Agent* agent);

    // Intersection* createIntersection(QPointF position);

    /**
     * @return controller at a given intersection, controllers enforce traffic rules on agents
     */
    const FlowController* controller(const Intersection* intersection) const;

    /**
     * @return router at a given intersection, routers stochastically route traffic
     */
    const FlowRouter* router(const Intersection* intersection) const;
    FlowRouter* router(const Intersection* intersection);

    /**
     * @return generator at a given intersection, generators generate set amount of vehicles per hour
     */
    const FlowGenerator *generator(const Intersection *intersection) const;
    FlowGenerator* generator(const Intersection* intersection);

    /**
     * @return amount of vehices which ended their route at intersection
     */
    int removedVehicleCount(const Intersection* intersection) const;

    // template<typename GeneratorType, typename... Args>
    // void addGenerator(Args&&... args);

    /**
     * @brief searches for lead vehicle of an agent against all agents in simulation, only works if both on the same traversable
     * @param agent (currently vehicle)
     * @return lead
     */
    const Vehicle* findLeadVehicle(const Agent* agent) const;

private:
    void populateIntersection(Intersection* intersection);
    void signalAgentAdded(const Agent* agent);
    void signalAgentRemoved(const Agent* agent);
};
