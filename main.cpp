#include "mainwindow.h"
#include "CyberStyle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(CyberStyle::globalStyleSheet());

    MainWindow w;
    w.show();
    return a.exec();
}
