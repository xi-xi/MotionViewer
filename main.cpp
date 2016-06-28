/*! \mainpage 簡易モーションビューワ
 *
 * とっても簡単に作った簡易的なモーションビューワ
 * 慌てて作ったので未実装な機能などが多い．
 * 一応見た目上は動くはず...
 */

#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include "motion.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.show();
    w.resize(1280, 960);
    if(a.arguments().size() > 1){
        w.openMotionFile(a.arguments()[1]);
    }

    return a.exec();
}
