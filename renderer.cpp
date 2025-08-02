#include "renderer.h"
#include "roadnetwork.h"
#include "intersection.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

Renderer::Renderer(QGraphicsScene *scene, RoadNetwork *network)
    : scene_{ scene }
    , network_{ network }
{}

void Renderer::draw()
{
    scene_->clear();
    qreal intersectionSize = 20.0;

    QPen roadPen(Qt::white, 2);
    for (const auto& roadway : network_->getRoadways())
    {
        QPointF p1 = roadway->getSource()->getPosition();
        QPointF p2 = roadway->getDestination()->getPosition();
        QLineF line = QLineF(p1, p2);

        if (roadway->getOppositeRoadway())
        {
            line = offsetLine(line, 5.0);
        }
        drawArrow(line, 15.0, 20.0, roadPen);
    }


    // Draw intersections
    for (const auto& intersection : network_->getIntersections())
    {
        QRectF rect(intersection->getPosition().x() - intersectionSize / 2, intersection->getPosition().y() - intersectionSize / 2, intersectionSize, intersectionSize);
        scene_->addEllipse(rect, QPen(Qt::white), QBrush(Qt::lightGray));
    }
}

void Renderer::drawArrow(QLineF baseline, qreal arrowheadLength, qreal arrowheadAngleDeg, QPen pen)
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

QLineF Renderer::offsetLine(const QLineF& line, qreal offset)
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
