#pragma once
#include <qtypes.h>
#include <random>

class Intersection;
class Traffic;
class GeometryManager;
class Roadway;
class Vehicle;

/**
 * @brief Generates agents based on hourly flow rate. Located at intersections with one outbound flow.
 */
class FlowGenerator
{
    qreal flow_;
    mutable std::minstd_rand rng_;
    mutable std::uniform_real_distribution<qreal> dist_;

    const Intersection* intersection_;
    const Roadway* flowSource_;

    const Vehicle* lastGeneratedVehicle_;
    int generatedCount_;
    int backlog_;

public:
    FlowGenerator(const Intersection* intersection);

    /**
     * @param hourly flow rate of vehicles generated
     */
    void flow(qreal value) { flow_ = value; }

    void update(qreal deltaTime, Traffic* traffic, const GeometryManager* geometry);

    /**
     * @brief check if flow rate is above 0
     */
    void validate();

    /**
     * @return total number of vehicles generated at this flow generator
     */
    qreal generatedCount() const { return generatedCount_; }

    /**
     * @return number of vehicles in backlog - meaning they were generated but didn't have enough space to be created and placed into the simulation
     */
    qreal backlog() const { return backlog_; }

private:
    bool shouldTrigger(qreal deltaTime) const;
    void establishFlowSource();
};

// class VehicleFlowGenerator : public FlowGenerator
// class PedestrianFlowGenerator : public FlowGenerator
