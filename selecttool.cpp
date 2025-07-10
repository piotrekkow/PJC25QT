#include "selecttool.h"
#include "simulationscene.h"
#include "objecthandle.h"

#include <QGraphicsSceneMouseEvent>

SelectTool::SelectTool(QObject *parent)
    : Tool(parent), itemBeingMoved(nullptr), isMoving(false) {}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    QGraphicsItem *item = scene->itemAt(event->scenePos(), QTransform());

    // If we clicked on an empty area, deselect everything
    if (!item) {
        if (currentlySelectedItem) {
            currentlySelectedItem->hideHandles(); // Tell old item to hide handles
            currentlySelectedItem = nullptr;
        }
        scene->clearSelection();
        return;
    }

    // If we clicked on a handle, do nothing. Let Qt's move logic take over.
    if (qgraphicsitem_cast<ObjectHandle*>(item)) {
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
        // Prepare to move the entire object if the user drags the body
        isMoving = true;
        originalPosition = baseItem->pos();
        pressPosition = event->scenePos();
    }
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    // Check if we are in the middle of a move
    if (isMoving && itemBeingMoved) {
        // Calculate the distance moved from the initial press
        QPointF offset = event->scenePos() - pressPosition;

        // Update the item's position
        itemBeingMoved->setPos(originalPosition + offset);
    }
    Q_UNUSED(scene);
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, SimulationScene *scene) {
    if (event->button() == Qt::LeftButton) {
        // Reset state variables
        itemBeingMoved = nullptr;
        isMoving = false;
    }
    Q_UNUSED(scene);
}
