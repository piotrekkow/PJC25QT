// main.cpp (modified)
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    return app.exec();
}


// QWidget *centralWidget = new QWidget(&window);
// window.setCentralWidget(centralWidget);

// QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget); // Use QHBoxLayout for example

// // Traffic view on one side
// SceneView *sceneView = new SceneView(centralWidget);
// mainLayout->addWidget(sceneView, 1); // Give it stretch factor 1

// // Control panel on the other side
// QMenuBar *menuBar = window.menuBar();
// QMenu *fileMenu = menuBar->addMenu("File");
// fileMenu->addAction("New");
// fileMenu->addAction("Open");
// fileMenu->addAction("Exit", &window, &QMainWindow::close);

// window.resize(1280, 720);
// window.setWindowTitle("PJC25 - Traffic Simulator");
