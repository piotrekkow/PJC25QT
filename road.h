#pragma once

#include "roadway.h"
#include <QPointF>

class Road
{
public:
    Road(const QPointF& start, const QPointF& end);

private:
    QPointF startPoint;
    QPointF endPoint;

    QVector<Roadway*> roadways_;
};
