#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <graphicsviewzoom.h>
#include <graphicsviewpannable.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view_ = ui->graphicsView;
    zoomController_ = new GraphicsViewZoom(view_);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);

    // large canvas (cad-like) to enable free panning
    qreal huge_dimension = 100000.0;
    scene_->setSceneRect(-huge_dimension, -huge_dimension, huge_dimension * 2, huge_dimension * 2);

    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    simulation_ = std::make_unique<Simulation>(scene_);

    simulationTimer_ = new QTimer(this);
    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::runSimulationUpdate);
    simulationTimer_->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runSimulationUpdate()
{
    double deltaTime = simulationTimer_->interval() / 1000.0;
    if (simulation_)
    {
        simulation_->update(deltaTime);
    }
}
