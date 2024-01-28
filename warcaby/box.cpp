#include "box.h"
#include <QFrame>
#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>

Box::Box(MainWindow *parent, int index): QFrame(parent)
{
    this->index = index;
    this->mainWindow = parent;
}

void Box::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->mainWindow->onClickBox(index);
    }
    QFrame::mousePressEvent(event);
}
