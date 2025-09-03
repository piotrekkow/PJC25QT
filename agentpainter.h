#pragma once

#include <memory>

class Agent;
class QGraphicsScene;
class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsTextItem;

/**
 * @brief Abstract base class for rendering agent objects.
 */
class AgentPainter {
public:
    virtual ~AgentPainter() = default;

    /// Called once to create all necessary QGraphicsItems for an agent object
    virtual void setup(const Agent* agent, QGraphicsScene* scene) = 0;

    /// Called every frame to render updated agent objects
    virtual void update(const Agent* agent) = 0;
};

struct VehicleRenderable
{
    QGraphicsRectItem* vehicleBody = nullptr;
    QGraphicsTextItem* debugText = nullptr;
};

class VehiclePainter : public AgentPainter {
public:
    VehiclePainter(bool debugMode);
    ~VehiclePainter() override; // We need a destructor for cleanup

    void setup(const Agent* agent, QGraphicsScene* scene) override;
    void update(const Agent* agent) override;

private:
    QGraphicsScene* scene_; // Store the scene for cleanup
    VehicleRenderable renderable_;
    bool debugMode_;
};

class PainterFactory {
public:
    /// Deduces agent type and creates a specific painter for given agent subclass
    std::unique_ptr<AgentPainter> createPainterFor(const Agent* agent, bool debugMode);
};
