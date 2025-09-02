#include "intersectionrouter.h"
#include "road.h"
#include "intersection.h"

void IntersectionRouter::addRoadwayFlows(const Roadway *from, std::vector<RoadwayFlow> flowsTo)
{
    if (from->destination() != intersection_)
        throw std::invalid_argument("Tried to pass a roadway which doesn't lead to router's intersection.");

    for (const auto& roadwayFlow : flowsTo)
    {
        if (roadwayFlow.roadway->source() != intersection_)
            throw std::invalid_argument("Tried to pass a roadway which doesn't start at router's intersection.");
    }

    routeFlows_.try_emplace(from, flowsTo);
}

const Roadway *IntersectionRouter::route(const Roadway *from) const
{
    auto it = routeFlows_.find(from);
    if (it == routeFlows_.end() || it->second.empty())
        return nullptr;

    const auto& flows = it->second;

    std::vector<int> weights;
    weights.reserve(flows.size());
    for (auto& f : flows)
        weights.push_back(f.flow);

    std::discrete_distribution<> dist(weights.begin(), weights.end());

    int idx = dist(rng_);
    return flows[idx].roadway;
}

void IntersectionRouter::validate() const
{
    // checks whether all incoming roadways have a set of flows (flows themselves might be incomplete eg. not include every roadway at an intersection)
    for (const auto& road : intersection_->roads())
    {
        if (intersection_->roads().size() <= 1) continue; // this is a dead end, no routing needed as it's the end of the route and vehicle will be marked for deletion.

        if (auto incomingRoadway = road->roadway(intersection_))
            if (!routeFlows_.contains(incomingRoadway))
                throw std::runtime_error("Tried to simulate an intersection without a route distribution for every incoming roadway.");
    }
}
