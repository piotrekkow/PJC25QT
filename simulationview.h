// #pragma once

// #include <QGraphicsView>
// #include <QMouseEvent>

// /**
//  * @brief Graphics view housing the simulation
//  */
// class SimulationView : public QGraphicsView
// {
//     Q_OBJECT

// public:
//     explicit SimulationView(QWidget* parent = nullptr);

// protected:
//     void wheelEvent(QWheelEvent* event) override;
//     void mousePressEvent(QMouseEvent* event) override;
//     void mouseMoveEvent(QMouseEvent* event) override;
//     void mouseReleaseEvent(QMouseEvent* event) override;

// private:
//     bool panning_;
//     QPoint lastPanPoint_;
// };
