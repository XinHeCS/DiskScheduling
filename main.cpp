#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiskScheduling w;
    w.show();

    return a.exec();
}
