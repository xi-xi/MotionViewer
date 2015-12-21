#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include "motion.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << a.arguments()[1];
    Motion m;
    m.open(a.arguments()[1]);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.setBaseSize(1280, 960);
    w.show();
    w.resize(1280, 960);

    return a.exec();
}
