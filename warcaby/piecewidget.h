#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H

#include <QWidget>

class PieceWidget : public QWidget
{
public:
    enum PieceColor
    {
        EMPTY,
        WHITE,
        BLACK
    };

    explicit PieceWidget(QWidget *parent, PieceColor color);
    void paintEvent(QPaintEvent *) override;
    void setMarked(int x)
    {
        marked = x;
    }

signals:

private:
    PieceColor pieceColor;
    bool marked = false;

};

#endif // PIECEWIDGET_H
