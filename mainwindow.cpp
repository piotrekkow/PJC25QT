#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QButtonGroup>
#include "roadtool.h"
#include "selecttool.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    setWindowTitle("PJC25");
    resize(1280, 720);

    // trafficSim = new TrafficSim(this);
    setupScene();
    setupView();
    setupDock();
    setupTools();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::setupConnections()
{
    connect(ui_->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui_->actionReset_zoom_and_pan, &QAction::triggered, zoomController_, &GraphicsViewZoom::resetZoom);
}

void MainWindow::setupDock()
{
    QAction* dockToggleAction = ui_->dockWidget->toggleViewAction();

    dockToggleAction->setText(ui_->actionDock->text());
    dockToggleAction->setShortcut(ui_->actionDock->shortcut());
    dockToggleAction->setCheckable(true);

    if (ui_->menuWindows)
    {
        if (ui_->menuWindows->actions().contains(ui_->actionDock))
        {
            ui_->menuWindows->removeAction(ui_->actionDock);
        }
        ui_->menuWindows->addAction(dockToggleAction);
    }
}

void MainWindow::setupScene()
{
    scene_ = new SimulationScene(this);
    scene_->setBackgroundBrush(QColor("#C1DBC1"));

    qreal huge_dimension = 100000;
    scene_->setSceneRect(-huge_dimension, -huge_dimension, huge_dimension * 2, huge_dimension * 2);
}

void MainWindow::setupView()
{
    view_ = ui_->graphicsView;
    zoomController_ = new GraphicsViewZoom(view_);

    view_->setScene(scene_);
    view_->setRenderHint(QPainter::Antialiasing); // Optional: for smoother rendering

    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::setupTools()
{
    QButtonGroup *toolGroup = new QButtonGroup(this);
    toolGroup->addButton(ui_->selectToolButton);
    toolGroup->addButton(ui_->roadToolButton);
    toolGroup->setExclusive(true);
}

void MainWindow::on_selectToolButton_toggled(bool checked)
{
    if (checked) {
        scene_->setTool(new SelectTool());
    }
}

void MainWindow::on_roadToolButton_toggled(bool checked)
{
    if (checked) {
        scene_->setTool(new RoadTool());
    }
}

