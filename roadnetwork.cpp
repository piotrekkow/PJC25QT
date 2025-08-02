#include "roadnetwork.h"
#include "intersection.h"
#include <QDebug>

RoadNetwork::RoadNetwork() {}

RoadNetwork::~RoadNetwork() = default;

Intersection *RoadNetwork::createIntersection(QPointF position)
{
    intersections_.emplace_back(std::make_unique<Intersection>(position));
    return intersections_.back().get();
}

Roadway *RoadNetwork::createRoadway(Intersection *source, Intersection *destination)
{
    roadways_.emplace_back(std::make_unique<Roadway>(source, destination));
    Roadway* newRoadway{ roadways_.back().get() };

    for (const auto& candidate : source->getIncomingRoadways())
    {
        if (candidate->getSource() == destination)
        {
            newRoadway->setOppositeRoadway(candidate);
            candidate->setOppositeRoadway(newRoadway);
            qDebug() << "Set 2 opposite roadways.";
            break;
        }
    }
    return newRoadway;
}

