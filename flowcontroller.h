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

class FlowController
{
protected:
    const Intersection* intersection_;

public:
    explicit FlowController(const Intersection* intersection) : intersection_{ intersection } {}
    virtual ~FlowController() = default;
    // virtual void update(qreal deltaTime, const std::vector<std::unique_ptr<Agent>>& agents);

    virtual std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const = 0;
    // virtual std::vector<ConflictData> conflictsMustYieldTo(const PedestrianCrossing* crossing, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const = 0;

protected:
    bool weHavePriorityInConflict(const Connection* us, const ConflictPoint* cp) const;
};

class SignController : public FlowController
{
public:
    using FlowController::FlowController;

    std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const override;
};
