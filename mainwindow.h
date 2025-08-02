#pragma once

#include <QMainWindow>
#include <QTimer>
#include "simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Simulation> simulation_;
    QTimer* simulationTimer_;
    QGraphicsScene* scene_;

private slots:
    void runSimulationUpdate();
};
