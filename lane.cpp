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

