#include "renderer.h"
#include "roadnetwork.h"
#include "intersection.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "geometrymanager.h"

Renderer::Renderer(QGraphicsScene *scene, const RoadNetwork *network)
    : scene_{ scene }
    , network_{ network }
{}

void Renderer::draw() const
{
    scene_->clear();
    auto geometry = network_->geometry();
    qreal intersectionSize = 25.0;
    // qreal cpSize = 2.0;

    QPen roadwayPen(Qt::yellow, 0.25);
    QPen connectionPen(Qt::white, 0.25);


    // Draw intersections
    for (const auto& intersection : network_->intersections())
    {
        drawCircle(intersection->position(), intersectionSize, QPen(Qt::darkYellow));

        for (const auto& connection : intersection->connections())
        {
            scene_->addPath(geometry->connection(connection.get()), connectionPen);
        }

        // for (const auto& cp : intersection->conflictManager()->conflicts())
        // {
        //     drawCircle(cp->position(), cpSize, QPen(Qt::red));
        // }

    }

    // Draw roads
    for (const auto& road : network_->roads())
    {
        for (const auto& roadway : road->roadways())
        {
            for (const auto& lane : roadway->lanes())
            {
                scene_->addPath(geometry->lane(lane.get()), QPen(Qt::gray, lane->width() * 0.5));
            }
            scene_->addPath(geometry->roadway(roadway), roadwayPen);
        }
    }

    for (const auto& vehicle : network_->vehicles())
    {
        QGraphicsRectItem* vehicleRect = new QGraphicsRectItem(
            -vehicle->length() / 2, -vehicle->width() / 2,
            vehicle->length(), vehicle->width()
            );
        vehicleRect->setPen(Qt::NoPen);
        vehicleRect->setBrush(QBrush(vehicle->color()));
        vehicleRect->setPos(vehicle->position());
        // The angle from QPainterPath is counter-clockwise, so we negate it.
        vehicleRect->setRotation(-vehicle->angle());
        scene_->addItem(vehicleRect);
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

void Renderer::drawCircle(QPointF position, qreal diameter, QPen pen) const
{
    QRectF rect(position.x() - diameter / 2, position.y() - diameter / 2, diameter, diameter);
    scene_->addEllipse(rect, pen);
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
