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

    /// Creates all necessary QGraphicsItems for an agent object
    virtual void setup(const Agent* agent, QGraphicsScene* scene) = 0;

    /// Called every frame to render updated agent objects
    virtual void update(const Agent* agent) = 0;
};

/**
 * @brief Holds graphics items of rendered angent objects
 */
struct VehicleRenderable
{
    QGraphicsRectItem* vehicleBody = nullptr;
    QGraphicsTextItem* debugText = nullptr;
};

/**
 * @brief Class for rendering vehicle objects
 */
class VehiclePainter : public AgentPainter {
public:
    VehiclePainter(bool debugMode);
    ~VehiclePainter() override; // We need a destructor for cleanup

    /// Creates all necessary QGraphicsItems for a vehicle object
    void setup(const Agent* agent, QGraphicsScene* scene) override;

    /// Called every frame to render updated vehicle objects
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



