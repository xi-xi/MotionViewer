#include "mainwindow.h"
#include "motionviewerwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    MotionViewerWidget *w = new MotionViewerWidget(this);
    this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}
