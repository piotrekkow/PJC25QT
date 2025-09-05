#include "agentpainter.h"
#include "vehicle.h"
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsItem>


VehiclePainter::VehiclePainter(bool debugMode)
    : scene_{ nullptr }
    , debugMode_{ debugMode }
{}

VehiclePainter::~VehiclePainter()
{
    if (scene_)
    {
        if (renderable_.vehicleBody)
        {
            scene_->removeItem(renderable_.vehicleBody);
            delete renderable_.vehicleBody;
        }
        if (renderable_.debugText)
        {
            scene_->removeItem(renderable_.debugText);
            delete renderable_.debugText;
        }
    }
}

void VehiclePainter::setup(const Agent* agent, QGraphicsScene* scene)
{
    scene_ = scene; // Keep a pointer to the scene for the destructor
    const auto* vehicle = static_cast<const Vehicle*>(agent);

    // Create the body
    renderable_.vehicleBody = new QGraphicsRectItem(-vehicle->length() / 2, -vehicle->width() / 2, vehicle->length(), vehicle->width());
    renderable_.vehicleBody->setPen(Qt::NoPen);
    renderable_.vehicleBody->setBrush(QBrush(Qt::cyan));
    scene_->addItem(renderable_.vehicleBody);

    // Create the debug text
    if (debugMode_) {
        renderable_.debugText = new QGraphicsTextItem();
        QFont font;
        font.setPointSize(2);
        renderable_.debugText->setFont(font);
        renderable_.debugText->setZValue(1);
        scene_->addItem(renderable_.debugText);
    }

    // Set initial state
    update(agent);
}

void VehiclePainter::update(const Agent* agent)
{
    renderable_.vehicleBody->setPos(agent->position());
    renderable_.vehicleBody->setRotation(-agent->angle());

    if (debugMode_ && renderable_.debugText) {
        const auto* vehicle = static_cast<const Vehicle*>(agent);

        std::string debugActionS;

        switch (vehicle->debugAction())
        {
        case DriverAction::Proceeding:
            debugActionS = "Proceeding";
            break;
        case DriverAction::Following:
            debugActionS = "Following";
            break;
        case DriverAction::Stopping:
            debugActionS = "Stopping";
            break;
        case DriverAction::Queueing:
            debugActionS = "Queueing";
            break;
        default:
            debugActionS = "Unknown";
            break;
        }

        QString agentDebugParams = QString::asprintf("%p: a = %.3f, \nv = %.3f, v_ = %.3f, \n%s",
                                                     (void *)vehicle,
                                                     vehicle->acceleration(),
                                                     vehicle->speed(),
                                                     vehicle->cruiseSpeed(),
                                                     debugActionS.c_str()
                                                     );

        renderable_.debugText->setPlainText(agentDebugParams);
        renderable_.debugText->setPos(agent->position() - QPointF{2.0, 5.0});
    }
}
std::unique_ptr<AgentPainter> PainterFactory::createPainterFor(const Agent *agent, bool debugMode)
{
    if (dynamic_cast<const Vehicle*>(agent)) return std::make_unique<VehiclePainter>(debugMode);
    // if (dynamic_cast<const Pedestrian*>(agent)) return std::make_unique<PedestrianPainter>(debugMode);
    // ...
    return nullptr;
}
