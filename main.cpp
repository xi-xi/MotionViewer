#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.setBaseSize(1280, 960);
    w.show();
    w.resize(1280, 960);

    return a.exec();
}
