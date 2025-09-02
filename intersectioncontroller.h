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

class IntersectionController
{
protected:
    const Intersection* intersection_;

public:
    explicit IntersectionController(const Intersection* intersection) : intersection_{ intersection } {}
    virtual ~IntersectionController() = default;
    // virtual void update(qreal deltaTime, const std::vector<std::unique_ptr<Agent>>& agents);

    // virtual bool canProceed(const Connection* conn, const Vehicle* vehicle, const std::vector<std::unique_ptr<Agent>>& agents) const = 0;
    // virtual bool canProceed(const PedestrianCrossing* crossing, const Pedestrian* pedestrian) const = 0;

    virtual std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const = 0;

protected:
    bool weHavePriorityInConflict(const Connection* us, const ConflictPoint* cp) const;
};

class SignController : public IntersectionController
{
public:
    using IntersectionController::IntersectionController;

    std::vector<ConflictData> conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Agent>>& agents, const GeometryManager* geometry) const override;
};
