#include "renderer.h"
#include "roadnetwork.h"
#include "intersection.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <iostream>

Renderer::Renderer(QGraphicsScene *scene, const RoadNetwork *network)
    : scene_{ scene }
    , network_{ network }
{}

void Renderer::draw() const
{
    scene_->clear();
    qreal intersectionSize = 5.0;

    QPen roadPen(Qt::yellow, 0.25);
    QPen roadwayPen(Qt::white, 0.5);
    // QPen connectionPen(Qt::green, 0.25);

    for (const auto& road : network_->roads())
    {
        QPointF startPos{ road->geometry().points().front().position() };
        QPointF endPos{ road->geometry().points().back().position() };

        QLineF roadLine = QLineF(startPos, endPos);
        scene_->addLine(roadLine, roadPen);

        for (const auto& roadway : road->roadways())
        {
            std::cerr << "roadway\n";
            for (const auto& lane : roadway->lanes())
            {
                std::cerr << "lane\n";
                QPolygonF laneGeometry = road->geometry().laneGeometry(lane.get());
                scene_->addPolygon(laneGeometry, roadwayPen);
            }
        }
    }


    // Draw intersections
    for (const auto& intersection : network_->intersections())
    {
        QRectF rect(intersection->position().x() - intersectionSize / 2, intersection->position().y() - intersectionSize / 2, intersectionSize, intersectionSize);
        scene_->addEllipse(rect, QPen(Qt::cyan));
    }
}



void Renderer::drawArrow(QLineF baseline, qreal arrowheadLength, qreal arrowheadAngleDeg, QPen pen) const
{
    if (qFuzzyCompare(baseline.length(), qreal(0.)))
        return;

    scene_->addLine(baseline, pen);

    qreal angle = baseline.angle();
    QPointF midpoint = baseline.pointAt(1.0);

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
