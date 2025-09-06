#pragma once

#include <random>
#include <unordered_map>

class Roadway;
class Intersection;

struct RoadwayFlow
{
    const Roadway* roadway;
    int flow;
};

class FlowRouter
{
    std::unordered_map<const Roadway*, std::vector<RoadwayFlow>> routeFlows_;
    mutable std::mt19937 rng_;  // mutable because next() is const
    const Intersection* intersection_;

public:
    FlowRouter(const Intersection* intersection);

    void addRoadwayFlows(const Roadway* from, std::vector<RoadwayFlow> flowsTo);

    const Roadway* route(const Roadway* from) const;
    void validate() const;

private:
    const std::vector<RoadwayFlow> roadwayFlows(const Roadway* from) const;

};
