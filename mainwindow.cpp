#include "mainwindow.h"
#include "motionviewerwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    MotionViewerWidget *w = new MotionViewerWidget(this);
    this->setCentralWidget(w);
    this->connect(this, SIGNAL(motionFileChanged(QString)), w, SLOT(openMotionFile(QString)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::openMotionFile(const QString &filename){
    emit this->motionFileChanged(filename);
}
