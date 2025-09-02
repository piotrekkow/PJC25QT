#include "renderer.h"
#include "roadnetwork.h"
#include "traffic.h"
#include "intersection.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "vehicle.h"

Renderer::Renderer(QGraphicsScene *scene, const RoadNetwork *network, const Traffic *traffic)
    : scene_{ scene }
    , network_{ network }
    , traffic_{ traffic }
{}

void Renderer::draw() const
{
    scene_->clear();
    auto geometry = network_->geometry();
    qreal intersectionSize = 25.0;
    qreal cpSize = 0.5;

    QPen roadwayPen(Qt::yellow, 0.25);
    QPen connectionPen(Qt::white, 0.25);
    QPen cpPen(Qt::red, 0.1);


    // Draw intersections
    for (const auto& intersection : network_->intersections())
    {
        drawCircle(intersection->position(), intersectionSize, QPen(Qt::darkYellow));

        for (const auto& connection : intersection->connections())
        {
            scene_->addPath(geometry->connection(connection.get()), connectionPen);
        }

        for (const auto& cp : intersection->conflictManager()->conflicts())
        {
            if (cp->classify() == ConflictPoint::ConflictType::Crossing) cpPen.setColor(Qt::red);
            else if (cp->classify() == ConflictPoint::ConflictType::Merging) cpPen.setColor(Qt::magenta);
            else cpPen.setColor(Qt::gray);

            const QPointF& pos = cp->position();
            drawCircle(pos, cpSize, cpPen);
            QString coordText = QString::asprintf("%.3f, %.3f", pos.x(), pos.y());
            QFont font;
            font.setPointSize(1);

            QGraphicsTextItem* textItem = scene_->addText(coordText);
            textItem->setFont(font);

            textItem->setPos(pos - QPointF{4.0, 4.0}); // Position the text at the same point
            textItem->setDefaultTextColor(Qt::red); // Set a visible color
        }

        int genCount = traffic_->generator(intersection.get())->generatedCount();
        QString generatedVehiclesCount = QString::asprintf("gen: %i", genCount);
        QFont font;
        font.setPointSize(4);
        QGraphicsTextItem* textItem = scene_->addText(generatedVehiclesCount);
        textItem->setFont(font);
        textItem->setPos(intersection->position() - QPointF{0.0, 0.0}); // Position the text at the same point
        textItem->setDefaultTextColor(Qt::green); // Set a visible color
    }

    // Draw roads
    for (const auto& road : network_->roads())
    {
        for (const auto& roadway : road->roadways())
        {
            // for (const auto& lane : roadway->lanes())
            // {
            //     scene_->addPath(geometry->lane(lane.get()), QPen(Qt::gray, lane->width() * 0.2));
            // }
            scene_->addPath(geometry->roadway(roadway), roadwayPen);
        }
    }

    for (const auto& agent : traffic_->agents())
    {
        QGraphicsRectItem* vehicleRect = new QGraphicsRectItem(
            -agent->length() / 2, -agent->width() / 2,
            agent->length(), agent->width()
            );
        vehicleRect->setPen(Qt::NoPen);
        vehicleRect->setBrush(QBrush(Qt::cyan));
        vehicleRect->setPos(agent->position());
        // The angle from QPainterPath is counter-clockwise, so we negate it.
        vehicleRect->setRotation(-agent->angle());
        scene_->addItem(vehicleRect);

        if (debug)
        {
            QString agentDebugParams = QString::asprintf("%p: a = %.3f, v = %.3f, vc = %.3f", (void *) agent.get(), agent->speed(), dynamic_cast<const Vehicle*>(agent.get())->acceleration(), dynamic_cast<const VehicleTraversable*>(agent->traversable())->speedLimit());
            QFont font;
            font.setPointSize(2);
            QGraphicsTextItem* textItem = scene_->addText(agentDebugParams);
            textItem->setFont(font);

            textItem->setPos(agent->position() - QPointF{4.0, 4.0});
            textItem->setDefaultTextColor(Qt::yellow);
        }
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
    QPointF direction = line.p2() - line.p1();
    QPointF normal(-direction.y(), direction.x());
    qreal length = std::hypot(normal.x(), normal.y());

    if (length == 0) return line;
    QPointF unitNormal = normal / length;
    QPointF offsetVector = unitNormal * offset;

    return QLineF(line.p1() + offsetVector, line.p2() + offsetVector);
}
