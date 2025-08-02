#pragma once

#include "roadway.h"
#include <QPointF>

class Intersection
{
    QPointF position_;
    std::vector<Roadway*> incomingRoadways_;
    std::vector<Roadway*> outgointRoadways_;

public:
    Intersection(QPointF position);
    void addRoadway(Roadway* roadway);

private:
    void addIncomingRoadway(Roadway* roadway);
    void addOutgoingRoadway(Roadway* roadway);
};
