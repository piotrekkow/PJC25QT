#include "roadway.h"
#include "intersection.h"
#include "road.h"
#include <QDebug>

Roadway::Roadway(const Road* parent, const Intersection *source, const Intersection *destination, PriorityType priority)
    : sourceIntersection_{ source }
    , destinationIntersection_{ destination }
    , road_{ parent }
    , speedLimit_{ 13.89 }
{
    this->priority(priority);
    addLane();
}

void Roadway::priority(PriorityType priority)
{
    if (!canHavePriority() && priority == PriorityType::Priority)
    {
        qDebug() << "Reached limit of (2) priority roadways entering intersection. Overwriting roadway " << this << " priority to yield instead.";
        priority = PriorityType::Yield;
    }
    priority_ = priority;
}

bool Roadway::isForwardRoadway() const
{
    return destinationIntersection_ == road_->endIntersection();
}

const Lane* Roadway::addLane()
{
    lanes_.emplace_back(std::make_unique<Lane>(this));
    return lanes_.back().get();
}

bool Roadway::canHavePriority()
{
    int priorityRoadwayLimit = 2;
    for (const auto& candidate : destinationIntersection_->roads())
    {
        const Roadway* incomingRoadway = candidate->roadwayInto(destinationIntersection_);

        if (incomingRoadway && incomingRoadway->priority() == PriorityType::Priority)
            priorityRoadwayLimit--;
    }

    return (priorityRoadwayLimit > 0);
}

