#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("PJC25");
    resize(1280, 720);

    // trafficSim = new TrafficSim(this);
    setupSceneView();
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
    QAction* dockToggleAction = ui->dockWidget->toggleViewAction();

    if (ui->actionDock) { // Check if ui->actionDock exists (it should from .ui file)
        dockToggleAction->setText(ui->actionDock->text());
        dockToggleAction->setShortcut(ui->actionDock->shortcut());
    }
    dockToggleAction->setCheckable(true);


    if (ui->menuWindows)
    {
        if (ui->actionDock && ui->menuWindows->actions().contains(ui->actionDock))
        {
            ui->menuWindows->removeAction(ui->actionDock);
        }
        ui->menuWindows->addAction(dockToggleAction);
    }
}

void MainWindow::setupSceneView()
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QColor("#2c2c2c"));

    QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 200, 100);
    rect->setBrush(QColor("green"));
    rect->setPen(QPen(Qt::black, 2));
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(rect);

    QGraphicsLineItem *line = new QGraphicsLineItem(50, 50, 250, 150);
    line->setPen(QPen(Qt::red, 3));
    scene->addItem(line);

    QGraphicsTextItem *text = new QGraphicsTextItem("Hello, 2D Scene!");
    text->setPos(10, 100);
    text->setDefaultTextColor(Qt::white);
    scene->addItem(text);

    view = new QGraphicsView(scene); // Create a QGraphicsView and associate it with the scene
    view->setRenderHint(QPainter::Antialiasing); // Optional: for smoother rendering
    view->setDragMode(QGraphicsView::RubberBandDrag); // Optional: enable selection with rubber band
    view->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true); // Performance hint

    setCentralWidget(view);
}

