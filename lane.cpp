#include "lane.h"
#include <stdexcept>
#include "intersection.h"
#include "roadway.h"

Lane::Lane(Roadway* parent, std::optional<float> length, float width)
    : width_{ width }
    , length_{ length }
    , roadway_{ parent }
{
}

IntersectionConnection *Lane::addConnection(Lane *target)
{
    if (!target)
        throw std::invalid_argument("Provided target lane invalid.");

    Intersection* upcomingIntersection = roadway_->destination();
    Intersection* targetsOutgoingIntersection = target->roadway_->source();

    if (upcomingIntersection != targetsOutgoingIntersection)
        throw std::invalid_argument("Provided target lane cannot form a valid connection.");

    connections_.emplace_back(std::make_unique<IntersectionConnection>(this, target));
    return connections_.back().get();
}

float Lane::cumulativeOffset() const
{
    float offset = 0.0f;
    const auto& lanes = roadway_->lanes();
    for (size_t i = 0; i < index(); ++i)
        offset += lanes[i]->width();
    return offset;
}

size_t Lane::index() const
{
    if (!roadway_)
        throw std::runtime_error("Lane has no parent roadway");

    const auto& lanes = roadway_->lanes(); // vector of unique_ptr<Lane>
    auto it = std::find_if(lanes.begin(), lanes.end(),
                           [this](const std::unique_ptr<Lane>& ptr){ return ptr.get() == this; });

    if (it == lanes.end())
        throw std::runtime_error("Lane not found in parent roadway");

    return static_cast<size_t>(std::distance(lanes.begin(), it));
}
