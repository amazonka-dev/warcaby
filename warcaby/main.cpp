#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>

int main(int argc, char *argv[])
{
    std::srand(std::time(0));
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    return app.exec();
}
