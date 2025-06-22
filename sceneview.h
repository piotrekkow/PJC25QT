#pragma once

#include <QWidget>
#include <QPainter>

class SceneView : public QWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};
