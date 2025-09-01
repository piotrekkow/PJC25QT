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

class IntersectionRouter
{
    std::unordered_map<const Roadway*, std::vector<RoadwayFlow>> routeFlows_;
    mutable std::mt19937 rng_;  // mutable because next() is const
    const Intersection* intersection_;

public:
    IntersectionRouter(const Intersection* intersection)
        : rng_(std::random_device{}()), intersection_{ intersection } {}

    void addRoadwayFlows(const Roadway* from, std::vector<RoadwayFlow> flowsTo);
    const Roadway* route(const Roadway* from) const;
    void validate() const;
};
