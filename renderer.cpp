#include "renderer.h"
#include "roadnetwork.h"
#include "intersection.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

Renderer::Renderer(QGraphicsScene *scene, const RoadNetwork *network)
    : scene_{ scene }
    , network_{ network }
{}

void Renderer::draw() const
{
    scene_->clear();
    qreal intersectionSize = 20.0;

    QPen roadPen(Qt::white, 2);
    QPen roadwayPen(Qt::yellow, 1);
    for (const auto& road : network_->getRoads())
    {
        QPointF primaryIntersectionPos{ road->getPrimaryIntersection()->getPosition() };
        QPointF secondaryIntersectionPos{ road->getSecondaryIntersection()->getPosition() };

        QLineF roadLine = QLineF(primaryIntersectionPos, secondaryIntersectionPos);
        scene_->addLine(roadLine, roadPen);

        QLineF primaryRoadwayLine = roadLine;
        QLineF secondaryRoadwayLine = QLineF(secondaryIntersectionPos, primaryIntersectionPos);

        if (road->getPrimaryRoadway() && road->getSecondaryRoadway())
        {
            primaryRoadwayLine = offsetLine(primaryRoadwayLine, 5.0);
            secondaryRoadwayLine = offsetLine(secondaryRoadwayLine, 5.0);
        }

        if (road->getPrimaryRoadway())
            drawArrow(primaryRoadwayLine, 15.0, 20.0, roadwayPen);

        if (road->getSecondaryRoadway())
            drawArrow(secondaryRoadwayLine, 15.0, 20.0, roadwayPen);

        for (const auto& lane : road->getPrimaryRoadway()->getLanesView())
        {
            primaryRoadwayLine = offsetLine(primaryRoadwayLine, lane->getWidth());
        }

        for (const auto& lane : road->getSecondaryRoadway()->getLanesView())
        {
            primaryRoadwayLine = offsetLine(secondaryRoadwayLine, lane->getWidth());
        }
    }


    // Draw intersections
    for (const auto& intersection : network_->getIntersections())
    {
        QRectF rect(intersection->getPosition().x() - intersectionSize / 2, intersection->getPosition().y() - intersectionSize / 2, intersectionSize, intersectionSize);
        scene_->addEllipse(rect, QPen(Qt::white), QBrush(Qt::lightGray));
    }
}

void Renderer::drawArrow(QLineF baseline, qreal arrowheadLength, qreal arrowheadAngleDeg, QPen pen) const
{
    if (qFuzzyCompare(baseline.length(), qreal(0.)))
        return;

    scene_->addLine(baseline, pen);

    qreal angle = baseline.angle();
    QPointF midpoint = baseline.pointAt(0.5);

    QLineF arrowHeadLine1(midpoint, QPointF());
    arrowHeadLine1.setLength(arrowheadLength);
    arrowHeadLine1.setAngle(angle + 180 + arrowheadAngleDeg);

    QLineF arrowHeadLine2(midpoint, QPointF());
    arrowHeadLine2.setLength(arrowheadLength);
    arrowHeadLine2.setAngle(angle + 180 - arrowheadAngleDeg);

    scene_->addLine(arrowHeadLine1, pen);
    scene_->addLine(arrowHeadLine2, pen);
}

QLineF Renderer::offsetLine(const QLineF& line, qreal offset) const
{
    // Step 1: get the direction vector of the line
    QPointF direction = line.p2() - line.p1();

    // Step 2: compute the normal vector (rotate 90 degrees counter-clockwise)
    QPointF normal(-direction.y(), direction.x());

    // Step 3: normalize the normal vector
    qreal length = std::hypot(normal.x(), normal.y());
    if (length == 0) return line; // avoid division by zero
    QPointF unitNormal = normal / length;

    // Step 4: scale by offset
    QPointF offsetVector = unitNormal * offset;

    // Step 5: move both endpoints
    return QLineF(line.p1() + offsetVector, line.p2() + offsetVector);
}
