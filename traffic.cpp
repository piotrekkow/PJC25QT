#include "traffic.h"
#include <vector>
#include "roadnetwork.h"
#include <QDebug>

Traffic::Traffic(RoadNetwork *network)
    : network_{ network }
{}

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

void Traffic::removeAgent(Agent *agent)
{
    agents_.erase(std::remove_if(agents_.begin(), agents_.end(),
                                 [agent](const std::unique_ptr<Agent>& a)
                                 {
                                     return a.get() == agent;
                                 }), agents_.end());
}

Intersection *Traffic::createIntersection(QPointF position)
{
    Intersection* intersection = network_->createIntersectionTopology(position);
    controllers_.emplace(std::make_pair(intersection, std::make_unique<SignController>(intersection)));
    routers_.emplace(std::make_pair(intersection, std::make_unique<IntersectionRouter>(intersection)));
    generators_.emplace(std::make_pair(intersection, std::make_unique<FlowGenerator>(intersection)));
    return intersection;
}

const IntersectionController *Traffic::controller(const Intersection *intersection) const
{
    auto it = controllers_.find(intersection);
    return (it != controllers_.end()) ? it->second.get() : nullptr;
}

const IntersectionRouter *Traffic::router(const Intersection *intersection) const
{
    auto it = routers_.find(intersection);
    return (it != routers_.end()) ? it->second.get() : nullptr;
}

IntersectionRouter *Traffic::router(const Intersection *intersection)
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
