#include "piecewidget.h"
#include <QPainter>
#include <QPen>

PieceWidget::PieceWidget(QWidget *parent, PieceColor color)
    : QWidget{parent}
{
    pieceColor = color;
}

void PieceWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(marked ? Qt::red : Qt::white);
    pen.setWidth(3);
    painter.setPen(pen);
    switch (pieceColor)
    {
        case EMPTY:
            painter.setBrush(Qt::transparent);
            break;
        case WHITE:
            painter.setBrush(Qt::white);
            break;
        case BLACK:
            painter.setBrush(Qt::black);
            break;
    }
    QRect parentRect = this->parentWidget()->rect();
    int radius = qMin(parentRect.width() - 4, parentRect.height() - 4) * 0.5;
    QPoint center(parentRect.width() / 2, parentRect.height() / 2);
    painter.drawEllipse(center, radius, radius);
}
