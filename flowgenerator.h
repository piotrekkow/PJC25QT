#pragma once
#include <qtypes.h>
#include <random>

class Intersection;
class Traffic;
class GeometryManager;
class Roadway;

class FlowGenerator
{
    qreal flow_;
    mutable std::minstd_rand rng_;
    mutable std::uniform_real_distribution<qreal> dist_;
    const Intersection* intersection_;
    const Roadway* flowSource_;
    int generatedCount_;

public:
    FlowGenerator(const Intersection* intersection);

    void flow(qreal value) { flow_ = value; }
    void update(qreal deltaTime, Traffic* traffic, const GeometryManager* geometry);
    void validate();
    qreal generatedCount() const { return generatedCount_; }

private:
    bool shouldTrigger(qreal deltaTime) const;
    void establishFlowSource();
};

// class VehicleFlowGenerator : public FlowGenerator
// class PedestrianFlowGenerator : public FlowGenerator
