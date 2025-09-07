#include "flowrouter.h"
#include "road.h"
#include "intersection.h"
#include <chrono>
#include <QDebug>



FlowRouter::FlowRouter(const Intersection *intersection)
    : rng_(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    , intersection_{ intersection }
{}

void FlowRouter::addRoadwayFlows(const Roadway *from, std::vector<RoadwayFlow> flowsTo)
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

const Roadway *FlowRouter::route(const Roadway *from) const
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

const std::vector<RoadwayFlow> FlowRouter::roadwayFlows(const Roadway *from) const
{
    auto it = routeFlows_.find(from);
    if (it != routeFlows_.end()) return it->second;
    else return {};
}

void FlowRouter::validate() const
{
    auto adjacency = intersection_->roadwayAdjacency();

    for (const auto& [fromRoad, flows] : routeFlows_)
    {
        // 1. Check flows sum
        int totalFlow = 0;
        for (auto& f : flows)
        {
            totalFlow += f.flow;

            // 2. Check connectivity
            if (!adjacency[fromRoad].count(f.roadway))
            {
                qDebug() << "Router: no flow set from " << fromRoad << " to " << f.roadway << ".";
            }
        }

        if (totalFlow <= 0)
        {
            qDebug() << "Router: total flow is 0 from roadway" << fromRoad;
        }
    }
}
