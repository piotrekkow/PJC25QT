#pragma once
#include <memory>
#include <qtypes.h>
#include <vector>

class Intersection;
class Agent;
class Vehicle;
class Connection;
class ConflictData;
class ConflictPoint;
class GeometryManager;

/**
 * @brief Controls priority rules at an intersection. Provides information to agents about which conflicts they have to resolve.
 */
class FlowController
{
protected:
    const Intersection* intersection_;

public:
    explicit FlowController(const Intersection* intersection) : intersection_{ intersection } {}
    virtual ~FlowController() = default;
    // virtual void update(qreal deltaTime, const std::vector<std::unique_ptr<Agent>>& agents);

    /**
     * @brief returns a vector of all agents with higher priority approaching all conflicts relevant to provided connection
     * @param conn connection which conflict points will be analyzed
     * @param agents all relevant agents that could possibly be in conflict
     * @param geometry
     * @return vector of conflict point and vector of agents approaching that conflict, latter of which includes not only the agent pointer but agent's distance to that conflict as well
     */
    virtual std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const = 0;
    // virtual std::vector<ConflictData> conflictsMustYieldTo(const PedestrianCrossing* crossing, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const = 0;

protected:
    /**
     * @brief considers regulatory priority and priority to the right to deduce whether provided connection has priority
     * @param us our connection
     * @param cp analyzed conflict point
     * @return true if we have priority
     */
    bool weHavePriorityInConflict(const Connection* us, const ConflictPoint* cp) const;
};

/**
 * @brief Controls rules at an intersection with just regulatory signage
 */
class SignController : public FlowController
{
public:
    using FlowController::FlowController;

    std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const override;
};
