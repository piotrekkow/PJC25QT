#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("PJC25");
    resize(1280, 720);

    // trafficSim = new TrafficSim(this);

    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    setupDock();
}

void MainWindow::setupDock()
{
    connect(ui->actionDock, &QAction::triggered, ui->dockWidget->toggleViewAction(), &QAction::trigger);
    connect(ui->dockWidget, &QDockWidget::visibilityChanged, ui->actionDock, &QAction::setChecked);

    ui->actionDock->setCheckable(true);
    ui->actionDock->setChecked(ui->dockWidget->isVisible());
    ui->actionDock->setShortcut(ui->dockWidget->toggleViewAction()->shortcut());
    connect(ui->actionDock, &QAction::triggered, ui->dockWidget, &QWidget::setVisible);

    ui->actionDock = ui->dockWidget->toggleViewAction();
    connect(ui->actionDock, &QAction::triggered, ui->dockWidget, &QWidget::setVisible);
    connect(ui->dockWidget, &QDockWidget::visibilityChanged, ui->actionDock, &QAction::setChecked);

    ui->actionDock->setChecked(ui->dockWidget->isVisible());
    connect(ui->actionDock, &QAction::triggered, [this](bool checked){
        ui->dockWidget->setVisible(checked);
    });

    connect(ui->dockWidget, &QDockWidget::visibilityChanged, ui->actionDock, &QAction::setChecked);
    ui->actionDock->setChecked(ui->dockWidget->isVisible());
}
