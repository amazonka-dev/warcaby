#ifndef BOX_H
#define BOX_H
#include <QFrame>
#include <QWidget>
#include "mainwindow.h"

class Box : public QFrame
{
public:
    Box(MainWindow *parent, int index);

    void mousePressEvent(QMouseEvent *event) override;
    void onClick();

private:
      int index;
      MainWindow *mainWindow;
};


#endif // BOX_H
