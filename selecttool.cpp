#include "selecttool.h"
#include "simulationscene.h"
#include "objecthandle.h"

#include <QGraphicsSceneMouseEvent>

SelectTool::SelectTool(QObject *parent)
    : Tool(parent)
    , currentlySelectedItem{ nullptr }
    , isMoving{ false }
{}

SelectTool::~SelectTool()
{
    if (currentlySelectedItem) {
        currentlySelectedItem->hideHandles();
    }
}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    QGraphicsItem *item = scene->itemAt(event->scenePos(), QTransform());

    // If we clicked on an empty area, deselect everything
    if (!item) {
        if (currentlySelectedItem) {
            currentlySelectedItem->hideHandles(); // Tell old item to hide handles
            currentlySelectedItem = nullptr;
        }
        scene->clearSelection();
        isMoving = false;
        return;
    }

    // If we clicked on a handle, do nothing. Let Qt's move logic take over.
    if (qgraphicsitem_cast<ObjectHandle*>(item)) {
        isMoving = false;
        return;
    }

    // If we clicked on a new BaseObject
    BaseObject *baseItem = qgraphicsitem_cast<BaseObject*>(item);
    if (baseItem) {
        // If we clicked on a different item than before
        if (baseItem != currentlySelectedItem) {
            if (currentlySelectedItem) {
                currentlySelectedItem->hideHandles(); // Hide old handles
            }
            scene->clearSelection();
            baseItem->setSelected(true);
            baseItem->showHandles(); // Tell new item to show handles
            currentlySelectedItem = baseItem;
        }
        if (baseItem->flags() & QGraphicsItem::ItemIsMovable) {
            isMoving = true;
            originalPosition = baseItem->pos();
            pressPosition = event->scenePos();
        } else {
            // Otherwise, explicitly ensure we are not in a moving state.
            isMoving = false;
        }
    } else {
        isMoving = false;
    }
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    // Check if we are in the middle of a move using the correct variable
    if (isMoving && currentlySelectedItem) {
        // Calculate the distance moved from the initial press
        QPointF offset = event->scenePos() - pressPosition;

        // Update the item's position
        currentlySelectedItem->setPos(originalPosition + offset);
    }
    Q_UNUSED(scene);
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    if (event->button() == Qt::LeftButton) {
        // We no longer need to manage itemBeingMoved here
        isMoving = false;
    }
    Q_UNUSED(scene);
}
