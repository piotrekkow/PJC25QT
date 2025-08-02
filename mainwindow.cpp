#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
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
