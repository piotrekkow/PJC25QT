#pragma once

#include <QGraphicsItem>
#include <QString>
#include <QFont>

class AlignedTextItem : public QGraphicsItem
{
    QString& text_;
    QFont& font_;
    QRectF bounds_;

public:
    AlignedTextItem(QString& text, QFont& font, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override { return bounds_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
