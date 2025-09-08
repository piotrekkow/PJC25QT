#pragma once

#include <qtypes.h>
#include <unordered_map>
#include "agentpainter.h"
#include "trafficobserver.h"

class RoadNetwork;
class Traffic;
class Agent;
class QGraphicsRectItem;
class QGraphicsTextItem;
class Intersection;
class QLineF;
class QPointF;
class QPen;
class QGraphicsItemGroup;
class GeometryManager;

/**
 * @brief Coordinates the rendering of objects, observes traffic in order to remove rendered dynamic graphic objects just before they're removed in Traffic class.
 */
class Renderer : public TrafficObserver
{
    std::unordered_map<const Agent*, std::unique_ptr<AgentPainter>> agentPainters_;
    std::unordered_map<const Intersection*, QGraphicsTextItem*> intersectionTextItems_;

    PainterFactory painterFactory_;
    QGraphicsItemGroup* staticLayer_;
    QGraphicsScene* scene_;

    bool debugMode_{ false };

public:
    Renderer(QGraphicsScene* scene);
    ~Renderer();

    /// One time set-up. Drawing the static road network layer.
    void initializeStaticLayer(const RoadNetwork* network);

    /// Per frame update. Drawing the dynamic traffic layer, drawing painter objects.
    void updateDynamicLayer(const Traffic* traffic, const RoadNetwork* network);

    /// Creates a new AgentPainter object when an agent is added
    void onAgentAdded(const Agent* agent) override;

    /// Removes a AgentPainter as agent is being removed
    void onAgentRemoved(const Agent* agent) override;

private:
    void drawStaticElements(const RoadNetwork* network);
    void drawStaticElementsDebug(const RoadNetwork* network);
    void drawConflicts(const Intersection* intersection) const;
    void drawRegulatorySigns(const Intersection* intersection, const GeometryManager *geometry) const;

    QGraphicsItemGroup* createStopSign() const;
    QGraphicsItemGroup* createYieldSign() const;
    QGraphicsItemGroup* createPrioritySign() const;
};

