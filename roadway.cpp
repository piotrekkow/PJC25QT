#include "roadway.h"
#include "road.h"
#include <QDebug>

Roadway::Roadway(Road* parent, Intersection *source, Intersection *destination, RoadwayDirection direction, PriorityType priority)
    : sourceIntersection_{ source }
    , destinationIntersection_{ destination }
    , direction_{ direction }
    , road_{ parent }
{
    this->priority(priority);
    addLane();
}

void Roadway::priority(PriorityType priority)
{
    if (priority == PriorityType::Priority)
    {
        if (!destinationIntersection_->reachedPriorityRoadwayLimit())
        {
            priority = PriorityType::Yield;
            qDebug() << "Lowered roadway priority level due to reaching limit (2) of priority roadways at intersection";
        }
    }
    priority_ = priority;
}


Lane* Roadway::addLane()
{
    lanes_.emplace_back(std::make_unique<Lane>(this));
    return lanes_.back().get();
}

