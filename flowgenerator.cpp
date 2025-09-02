#include "flowgenerator.h"
#include <qdebug.h>
#include "traffic.h"
#include "intersection.h"
#include "road.h"
#include "vehicle.h"

FlowGenerator::FlowGenerator(const Intersection *intersection)
    : flow_{ 0 }
    , rng_(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    , intersection_{ intersection }
    , generatedCount_{ 0 }
{}

void FlowGenerator::update(qreal deltaTime, Traffic* traffic, const GeometryManager* geometry)
{
    if (shouldTrigger(deltaTime) && flowSource_)
    {
        // TODO: logic to spread out vehicles if roadway has more lanes, integration with router so that vehicle more likely to appear at lane connecting to where it's being routed
        traffic->createAgent<Vehicle>(flowSource_->lanes()[0].get(), traffic, geometry);
        generatedCount_++;
    }
}

void FlowGenerator::validate()
{
    if (flow_ == 0) qDebug() << "WARNING: Intersection " << intersection_ << " is designated as a flow generator but generates 0 vehicles per hour.";
    establishFlowSource();
}

bool FlowGenerator::shouldTrigger(qreal deltaTime) const
{
    qreal probability = deltaTime * flow_ / 3600.0;
    return dist_(rng_) < probability;
}

void FlowGenerator::establishFlowSource()
{
    const Roadway* candidate{ nullptr };
    if (!intersection_->roads().empty())
    {
        for (const auto& road : intersection_->roads())
        {
            for (const auto& roadway : road->roadways())
            {
                if (roadway->source() == intersection_)
                {
                    if (!candidate)
                    {
                        candidate = roadway;
                    }
                    else
                    {
                        qDebug() << "Couldn't establish a flow source because intersection " << intersection_ << " has more than 1 outgoing roadway.";
                        return;
                    }
                }
            }
        }
    }

    if (candidate)
        flowSource_ = candidate;
    else
        qDebug() << "Couldn't establish a flow source because intersection " << intersection_ << " has no outgoing roadway.";
}
