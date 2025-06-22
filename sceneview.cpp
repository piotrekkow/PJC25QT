#include "sceneview.h"

SceneView::SceneView(QWidget *parent)
    : QWidget{parent}
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::lightGray);
    setPalette(pal);
}

void SceneView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::darkGray, 10));
    painter.drawLine(50, height() / 2, width() - 50, height() / 2);
}
