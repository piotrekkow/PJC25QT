#include "simulationscene.h"

SimulationScene::SimulationScene(QObject *parent)
    : QGraphicsScene{ parent }
    , currentTool_{ nullptr }
{}

SimulationScene::~SimulationScene() {
    // The scene now owns the tool, so it should delete it
    if (currentTool_) {
        delete currentTool_;
    }
}

void SimulationScene::setTool(Tool *tool) {
    // Delete the old tool before setting the new one
    if (currentTool_) {
        delete currentTool_;
    }
    currentTool_ = tool;
}

void SimulationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool_) {
        currentTool_->mousePressEvent(event, this);
    }
    QGraphicsScene::mousePressEvent(event);
}

void SimulationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool_) {
        currentTool_->mouseMoveEvent(event, this);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void SimulationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool_) {
        currentTool_->mouseReleaseEvent(event, this);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
