#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graphicsviewzoom.h"
#include "simulationscene.h"

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

private slots:
    void on_selectToolButton_toggled(bool checked);

    void on_roadToolButton_toggled(bool checked);

private:
    Ui::MainWindow* ui_;
    SimulationScene* scene_;
    QGraphicsView* view_;
    GraphicsViewZoom* zoomController_;

    void setupConnections();
    void setupDock();
    void setupSceneView();
    void setupScene();
    void setupView();
    void setupTools();
};
