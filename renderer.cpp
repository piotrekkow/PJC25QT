#include "renderer.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <qpainter.h>

#include "roadnetwork.h"
#include "traffic.h"
#include "agent.h"
#include "vehicle.h"

#include "intersection.h"


Renderer::Renderer(QGraphicsScene *scene)
    : painterFactory_()
    , staticLayer_{ nullptr } // Initialize pointer
    , scene_{ scene }
{}

Renderer::~Renderer() {}

void Renderer::initializeStaticLayer(const RoadNetwork *network)
{
    staticLayer_ = new QGraphicsItemGroup();
    staticLayer_->setZValue(-10);
    scene_->addItem(staticLayer_);

    debugMode_ ? drawStaticElementsDebug(network)
               : drawStaticElements(network);

    staticLayer_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void Renderer::updateDynamicLayer(const Traffic *traffic, const RoadNetwork *network)
{
    // 1. Update all agent visuals
    for (const auto& agentPtr : traffic->agents()) {
        auto it = agentPainters_.find(agentPtr.get());
        if (it != agentPainters_.end()) {
            it->second->update(agentPtr.get());
        }
    }

    // 2. Update all dynamic intersection text
    for (const auto& intersectionPtr : network->intersections())
    {
        const Intersection* intersection = intersectionPtr.get();
        QStringList dataParts;

        // First condition (gen)
        if (auto gen = traffic->generator(intersection))
        {
            int genCount = gen->generatedCount();
            int backlog = gen->backlog();
            dataParts.append(QString("gen: %1\nbacklog: %2").arg(genCount).arg(backlog));
        }

        if (int rmCount = traffic->removedVehicleCount(intersection); rmCount > 0)
        {
            dataParts.append(QString("rm: %1").arg(rmCount));
        }

        QString intersectionData = dataParts.join('\n');

        auto it = intersectionTextItems_.find(intersection);
        if (it != intersectionTextItems_.end())
        {
            it->second->setPlainText(intersectionData);
        }
        else
        {
            QFont font;
            font.setPointSize(4);
            auto textItem = new QGraphicsTextItem(intersectionData);
            textItem->setFont(font);
            textItem->setPos(intersection->position());
            textItem->setDefaultTextColor(Qt::green);
            textItem->setZValue(10);
            scene_->addItem(textItem);
            intersectionTextItems_[intersection] = textItem;
        }
    }
}

void Renderer::onAgentAdded(const Agent* agent)
{
    auto painter = painterFactory_.createPainterFor(agent, debugMode_);
    if (painter) {
        painter->setup(agent, scene_);
        agentPainters_[agent] = std::move(painter);
    }
}

void Renderer::onAgentRemoved(const Agent* agent)
{
    // ~VehiclePainter() automatically cleans up the QGraphicsItems.
    agentPainters_.erase(agent);
}

void Renderer::drawConflicts(const Intersection *intersection) const
{
    QPen cpPen(Qt::red, 0.1);
    qreal cpSize = 0.5;

    for (const auto& cp : intersection->conflictManager()->conflicts())
    {
        if (cp->classify() == ConflictPoint::ConflictType::Crossing) cpPen.setColor(Qt::red);
        else if (cp->classify() == ConflictPoint::ConflictType::Merging) cpPen.setColor(Qt::magenta);
        else cpPen.setColor(Qt::gray);

        const QPointF& pos = cp->position();
        QRectF rect(pos.x() - cpSize / 2, pos.y() - cpSize / 2, cpSize, cpSize);
        auto ellipse = new QGraphicsEllipseItem(rect);
        ellipse->setPen(cpPen);
        staticLayer_->addToGroup(ellipse);

        QString coordText = QString::asprintf("%.3f, %.3f", pos.x(), pos.y());
        QFont font;
        font.setPointSizeF(0.5);

        auto textItem = new QGraphicsTextItem(coordText);
        textItem->setFont(font);
        textItem->setPos(pos - QPointF{4.0, 4.0});
        textItem->setDefaultTextColor(Qt::red);
        staticLayer_->addToGroup(textItem);
    }
}

void Renderer::drawStaticElements(const RoadNetwork *network)
{
    auto geometry = network->geometry();
    QPen roadwayMarkingPen(Qt::yellow, 0.25);
    QPen asphaltPen(Qt::gray, 3.8);

    for (const auto& road : network->roads())
    {
        for (const auto& roadway : road->roadways())
        {
            for (const auto& lane : roadway->lanes())
            {
                auto pathItem = new QGraphicsPathItem(geometry->lane(lane.get()));
                pathItem->setPen(asphaltPen);
                staticLayer_->addToGroup(pathItem);
            }

            if (road->roadways().size() == 2)
            {
                auto pathItem = new QGraphicsPathItem(geometry->roadway(roadway));
                pathItem->setPen(roadwayMarkingPen);
                staticLayer_->addToGroup(pathItem);
            }
        }
    }

    for (const auto& intersection : network->intersections())
    {
        for (const auto& connection : intersection->connections())
        {
            const QPainterPath& connectionPath = geometry->connection(connection.get());
            auto pathItem = new QGraphicsPathItem(connectionPath);
            pathItem->setPen(asphaltPen);
            staticLayer_->addToGroup(pathItem);

            qreal offset = connection->stopLineOffset();
            qreal stopPercent = (offset == 0.0) ? 0.0 : offset / connectionPath.length();
            QPointF stopPoint = connectionPath.pointAtPercent(stopPercent);
            QPointF p2 = connectionPath.pointAtPercent(stopPercent + 0.01);
            QLineF tangent(stopPoint, p2);
            QLineF normal = tangent.normalVector();
            normal.setLength(asphaltPen.widthF() * 0.45);
            normal.setPoints(normal.p2(), normal.p1());
            normal.setLength(asphaltPen.widthF() * 0.9);

            auto stopLineItem = new QGraphicsLineItem(normal);
            stopLineItem->setPen(roadwayMarkingPen);
            stopLineItem->setZValue(5.0);

            staticLayer_->addToGroup(stopLineItem);
        }

        drawRegulatorySigns(intersection.get(), geometry);
    }
}

void Renderer::drawStaticElementsDebug(const RoadNetwork *network)
{
    auto geometry = network->geometry();
    qreal intersectionSize = 25.0;
    QPen connectionPen(Qt::gray, 0.25);

    for (const auto& intersection : network->intersections())
    {
        QRectF rect(intersection->position().x() - intersectionSize / 2, intersection->position().y() - intersectionSize / 2, intersectionSize, intersectionSize);
        auto ellipse = new QGraphicsEllipseItem(rect);
        ellipse->setPen(QPen(Qt::darkYellow));
        staticLayer_->addToGroup(ellipse);

        for (const auto& connection : intersection->connections())
        {
            auto pathItem = new QGraphicsPathItem(geometry->connection(connection.get()));
            pathItem->setPen(connectionPen);
            staticLayer_->addToGroup(pathItem);
        }

        drawConflicts(intersection.get());
        drawRegulatorySigns(intersection.get(), geometry);
    }

    QPen roadwayPen(Qt::yellow, 0.25);

    for (const auto& road : network->roads())
    {
        for (const auto& roadway : road->roadways())
        {
            auto pathItem = new QGraphicsPathItem(geometry->roadway(roadway));
            pathItem->setPen(roadwayPen);
            staticLayer_->addToGroup(pathItem);
        }
        for (const auto& rpoint : road->geometry().points())
        {

            QRectF rect(rpoint.position().x() - 2.0 / 2, rpoint.position().y() - 2.0 / 2, 2.0, 2.0);
            auto ellipse = new QGraphicsEllipseItem(rect);
            ellipse->setPen(QPen(Qt::blue));
            staticLayer_->addToGroup(ellipse);
        }
    }
}

void Renderer::drawRegulatorySigns(const Intersection *intersection, const GeometryManager* geometry) const
{
    if (intersection->roads().size() < 2) return;
    for (const auto& road : intersection->roads())
    {
        const Roadway* incomingRoadway = road->roadwayInto(intersection);
        if (incomingRoadway)
        {
            QGraphicsItemGroup* sign = nullptr;

            switch (incomingRoadway->priority()) {
            case PriorityType::Stop:
                sign = createStopSign();
                break;
            case PriorityType::Yield:
                sign = createYieldSign();
                break;
            case PriorityType::Priority:
                sign = createPrioritySign();
                break;
            }

            if (sign) {
                const auto& lastLane = incomingRoadway->lanes().back();
                const QPainterPath& lanePath = geometry->lane(lastLane.get());

                if (!lanePath.isEmpty()) {
                    QPointF roadEndPosition = lanePath.pointAtPercent(1.0);
                    QPointF p1 = lanePath.pointAtPercent(0.99);
                    QLineF tangent(p1, roadEndPosition);
                    QLineF normal = tangent.normalVector();
                    qreal signOffset = -6.0;
                    normal.setLength(signOffset);

                    QPointF signPosition = normal.p2();

                    qreal signAngle = lanePath.angleAtPercent(1.0) + 90;


                    sign->setPos(signPosition);
                    sign->setRotation(-signAngle);

                    staticLayer_->addToGroup(sign);
                }
                else
                {
                    delete sign;
                }
            }
        }
    }
}

QGraphicsItemGroup* Renderer::createStopSign() const
{
    auto group = new QGraphicsItemGroup();
    qreal size = 6.2; // diameter

    // 1. Create the octagon shape
    QPolygonF octagon;
    for (int i = 0; i < 8; ++i) {
        octagon << QPointF(size / 2 * cos(M_PI / 8 + i * M_PI / 4),
                           size / 2 * sin(M_PI / 8 + i * M_PI / 4));
    }
    auto octagonItem = new QGraphicsPolygonItem(octagon);
    octagonItem->setBrush(Qt::red);
    octagonItem->setPen(QPen(Qt::white, 0.2));
    group->addToGroup(octagonItem);

    // 2. Create the "STOP" text
    auto textItem = new QGraphicsTextItem("STOP");
    textItem->setFont(QFont("Bebas Neue", 2, QFont::Bold));
    textItem->setDefaultTextColor(Qt::white);

    QRectF textBounds = textItem->boundingRect();
    textItem->setPos(textBounds.width() / 2, textBounds.height() / 2.1);
    textItem->setRotation(180);
    group->addToGroup(textItem);

    return group;
}

QGraphicsItemGroup* Renderer::createYieldSign() const
{
    auto group = new QGraphicsItemGroup();
    qreal height = 5.0;
    qreal width = height * 1.15;

    QPolygonF triangle;
    triangle << QPointF(0, -height / 2) << QPointF(-width / 2, height / 2) << QPointF(width / 2, height / 2);

    auto mainSignItem = new QGraphicsPolygonItem(triangle);
    mainSignItem->setBrush(Qt::yellow);
    mainSignItem->setPen(QPen(Qt::red, height / 10.0));
    group->addToGroup(mainSignItem);
    return group;
}

QGraphicsItemGroup* Renderer::createPrioritySign() const
{
    auto group = new QGraphicsItemGroup();
    qreal size = 4.5;

    // Outer white diamond with black border
    auto outerRect = new QGraphicsRectItem(-size / 2, -size / 2, size, size);
    outerRect->setBrush(Qt::white);
    outerRect->setPen(QPen(Qt::black, size / 20.0));
    outerRect->setRotation(45.0);
    group->addToGroup(outerRect);

    // Inner yellow diamond
    qreal innerSize = size * 0.6;
    auto innerRect = new QGraphicsRectItem(-innerSize / 2, -innerSize / 2, innerSize, innerSize);
    innerRect->setBrush(Qt::yellow);
    innerRect->setPen(QPen(Qt::black, size / 30.0));
    innerRect->setRotation(45.0);
    group->addToGroup(innerRect);

    return group;
}
