#include "alignedtextitem.h"
#include <QPainter>
#include <QFontMetricsF>

AlignedTextItem::AlignedTextItem(QString &text, QFont &font, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , text_{ text }
    , font_{ font }
{
    QFontMetricsF fm(font_);
    bounds_ = fm.boundingRect(text_);
}

void AlignedTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setFont(font_);
    painter->setPen(Qt::white);

    painter->drawText(bounds_, Qt::AlignRight, text_);
}
