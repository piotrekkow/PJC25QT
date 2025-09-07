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

/**
 * @brief Stochastic router located at intersections where multiple flows meet
 */
class FlowRouter
{
    std::unordered_map<const Roadway*, std::vector<RoadwayFlow>> routeFlows_;
    mutable std::mt19937 rng_;  // mutable because next() is const
    const Intersection* intersection_;

public:
    FlowRouter(const Intersection* intersection);

    /**
     * @brief addRoadwayFlows
     * @param from roadway from which flow originates
     * @param flowsTo vector of roadway and flow flowing into that roadway
     */
    void addRoadwayFlows(const Roadway* from, std::vector<RoadwayFlow> flowsTo);

    /**
     * @brief called once to return a random destination
     * @param from provided roadway
     * @return roadway to which the flow from provided roadway should be routed
     */
    const Roadway* route(const Roadway* from) const;

    /**
     * @brief check whether all roadways which are connected have set flows
     */
    void validate() const;

private:
    const std::vector<RoadwayFlow> roadwayFlows(const Roadway* from) const;

};
