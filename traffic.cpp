#include "traffic.h"
#include "vehicle.h"

#include "roadnetwork.h"
#include "intersection.h"

#include <vector>
#include <QDebug>

Traffic::Traffic(RoadNetwork *network)
    : network_{ network }
{
    for (const auto& intersection : network_->intersections())
    {
        populateIntersection(intersection.get());
    }
}

void Traffic::update(qreal deltaTime)
{
    std::vector<Agent*> agentsToRemove;
    for (const auto& agent : agents_)
    {
        agent->update(deltaTime);

        if (agent->isMarkedForRemoval())
            agentsToRemove.push_back(agent.get());
    }

    for (const auto& agent : agentsToRemove)
    {
        removeAgent(agent);
    }

    for (const auto& generator : generators_)
    {
        generator.second->update(deltaTime, this, network_->geometry());
    }
}

void Traffic::validate()
{
    for (const auto& generator : generators_) generator.second->validate();
    for (const auto& router : routers_) router.second->validate();
}

void Traffic::removeAgent(Agent *agent)
{
    signalAgentRemoved(agent);

    if (const VehicleTraversable* traversable = dynamic_cast<const VehicleTraversable*>(agent->traversable()))
    {
        if (const Intersection* intersection = traversable->intersection())
            removedVehicleCounts_[intersection]++;
    }

    agents_.erase(std::remove_if(agents_.begin(), agents_.end(),
                                 [agent](const std::unique_ptr<Agent>& a)
                                 {
                                     return a.get() == agent;
                                 }), agents_.end());
}

const FlowController *Traffic::controller(const Intersection *intersection) const
{
    auto it = controllers_.find(intersection);
    return (it != controllers_.end()) ? it->second.get() : nullptr;
}

const FlowRouter *Traffic::router(const Intersection *intersection) const
{
    auto it = routers_.find(intersection);
    return (it != routers_.end()) ? it->second.get() : nullptr;
}

FlowRouter *Traffic::router(const Intersection *intersection)
{
    auto it = routers_.find(intersection);
    return (it != routers_.end()) ? it->second.get() : nullptr;
}

const FlowGenerator *Traffic::generator(const Intersection *intersection) const
{
    auto it = generators_.find(intersection);
    return (it != generators_.end()) ? it->second.get() : nullptr;
}

FlowGenerator *Traffic::generator(const Intersection *intersection)
{
    auto it = generators_.find(intersection);
    return (it != generators_.end()) ? it->second.get() : nullptr;
}

int Traffic::removedVehicleCount(const Intersection *intersection) const
{
    auto it = removedVehicleCounts_.find(intersection);
    if (it != removedVehicleCounts_.end())
        return it->second;
    return 0;
}

const Vehicle* Traffic::findLeadVehicle(const Agent *agent) const
{
    const Agent* leadVehicle = nullptr;
    qreal minDistance = std::numeric_limits<qreal>::max();

    for (const auto& candidate : agents_)
    {
        if (agent == candidate.get()) continue;

        if (agent->traversable() == candidate->traversable() && candidate->progress() > agent->progress())
        {
            qreal distance = candidate->progress() - agent->progress();
            if (distance < minDistance)
            {
                minDistance = distance;
                leadVehicle = candidate.get();
            }
        }
    }
    return static_cast<const Vehicle*>(leadVehicle);
}

void Traffic::populateIntersection(Intersection *intersection)
{
    int roads = intersection->roads().size();
    if (roads > 1)
    {
        controllers_.emplace(std::make_pair(intersection, std::make_unique<SignController>(intersection)));
        routers_.emplace(std::make_pair(intersection, std::make_unique<FlowRouter>(intersection)));
    }
    else if (roads == 1)
    {
        for (const auto& roadway : intersection->roads()[0]->roadways())
        {
            if (roadway->source() == intersection)
                generators_.emplace(std::make_pair(intersection, std::make_unique<FlowGenerator>(intersection)));

            if (roadway->destination() == intersection)
                removedVehicleCounts_.emplace(std::make_pair(intersection, 0));
        }
    }
    else
    {
        qDebug() << "Intersection " << this << " at "
                 << intersection->position().x() << ", "
                 << intersection->position().y() << " has no connecting topology.";
    }
}

void Traffic::signalAgentAdded(const Agent* agent) {
    for (auto* observer : observers_) {
        observer->onAgentAdded(agent);
    }
}

void Traffic::signalAgentRemoved(const Agent* agent) {
    for (auto* observer : observers_) {
        observer->onAgentRemoved(agent);
    }
}
