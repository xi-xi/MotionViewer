#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include "motionviewerwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    MotionViewerWidget *w = new MotionViewerWidget(this);
    this->setCentralWidget(w);
    this->connect(this, SIGNAL(motionFileChanged(QString)), w, SLOT(openMotionFile(QString)));

    this->setWindowTitle("Motion Viewer");

    this->filemenu = new QMenu(this);
    this->openaction = new QAction("&Open", this);
    this->closeaction = new QAction("Close", this);
    filemenu->addAction(this->openaction);
    filemenu->addSeparator();
    filemenu->addAction(this->closeaction);
    this->menuBar()->addMenu(filemenu);

    this->helpmenu = new QMenu(this);
    this->aboutcaction = new QAction(this);
    this->addAction(this->aboutcaction);
    this->menuBar()->addMenu(this->helpmenu);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openMotionFile(const QString &filename){
    emit this->motionFileChanged(filename);
}
