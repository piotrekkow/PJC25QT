#pragma once

#include "roadway.h"
#include <QPointF>

class Intersection
{
    QPointF position_;
    std::vector<Roadway*> incomingRoadways_;
    std::vector<Roadway*> outgoingRoadways_;

public:
    Intersection(QPointF position);

    const std::vector<Roadway*>& getIncomingRoadways() const { return incomingRoadways_; }
    const std::vector<Roadway*>& getOutgoingRoadways() const { return outgoingRoadways_; }

    void addIncomingRoadway(Roadway* roadway);
    void addOutgoingRoadway(Roadway* roadway);
    QPointF getPosition() const { return position_; }

private:

};
