#pragma once
#include <qtypes.h>
#include <vector>
#include <memory>

class Vehicle;
class GeometryManager;
class ConflictPoint;
class IntersectionDecisionData;
class Connection;

class IIntersectionController
{
public:
    virtual ~IIntersectionController() = default;
    virtual const IntersectionDecisionData decisionData(const Vehicle* vehicle, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) const = 0;
    // virtual IntersectionDecisionData conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) = 0;
};

class Intersection;
class Connection;

class UncontrolledIntersection : public IIntersectionController
{
    const Intersection* intersection_;
    const GeometryManager* geometry_;

public:
    explicit UncontrolledIntersection(const Intersection* intersection, const GeometryManager* geometry)
        : intersection_{ intersection }
        , geometry_{ geometry }
    {}
    const IntersectionDecisionData decisionData(const Vehicle* vehicle, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) const override;
    // IntersectionDecisionData conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) override;

private:
    //bool mustYieldTo(const Connection* self, const Connection* other);
    IntersectionDecisionData const conflictsMustYieldTo(const Connection* conn, const std::vector<std::unique_ptr<Vehicle>>& allVehicles) const;
};
