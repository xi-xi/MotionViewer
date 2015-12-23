#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QStyle>
#include <QLabel>

#include "motionviewerwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->viewerwidget = new MotionViewerWidget(this);
    this->filemenu = this->menuBar()->addMenu("File");
    this->openaction = new QAction("Open", this);
    this->openaction->setShortcuts(QKeySequence::Open);
    this->configureaction = new QAction("Preferences", this);
    this->closeaction = new QAction("Close", this);
    this->closeaction->setShortcuts(QKeySequence::Close);
    this->operationbar = new QToolBar("Operation", this);
    this->operationbar->setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);

    this->back = new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaSkipBackward), "Back", this);
    this->play_stop = new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaPlay),"Play", this);
    this->next = new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaSkipForward), "Next", this);

    this->filename = new QLabel("Not Opened",this);
    this->initUI();
    this->initLogic();

    this->setWindowTitle("Motion Viewer");
}

void MainWindow::initUI()
{
    this->setCentralWidget(this->viewerwidget);
    this->filemenu->addAction(this->openaction);
    this->filemenu->addAction(this->configureaction);
    this->filemenu->addSeparator();
    this->filemenu->addAction(this->closeaction);
    this->addToolBar(Qt::BottomToolBarArea, this->operationbar);
    this->operationbar->addAction(this->back);
    this->operationbar->addAction(this->play_stop);
    this->operationbar->addAction(this->next);
    this->operationbar->addWidget(this->filename);
}

void MainWindow::initLogic()
{
    this->connect(this, SIGNAL(motionFileChanged(QString)), this, SLOT(onMotionFileChanged(QString)));
    this->connect(this, SIGNAL(motionFileChanged(QString)), this->viewerwidget, SLOT(openMotionFile(QString)));
    this->connect(this->openaction, SIGNAL(triggered()), this, SLOT(openFile()));
    this->connect(this->configureaction, SIGNAL(triggered()), this, SLOT(configure()));
    this->connect(this->closeaction, SIGNAL(triggered()), this, SLOT(close()));
    this->connect(this->back, SIGNAL(triggered()), this, SLOT(backButtonClicked()));
    this->connect(this->play_stop, SIGNAL(triggered()), this, SLOT(playstopButtonClicked()));
    this->connect(this->next, SIGNAL(triggered()), this, SLOT(nextButtonClicked()));
    this->connect(this->viewerwidget, SIGNAL(playStateChanged(bool)), this, SLOT(setPlayStopButtonState(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::openMotionFile(const QString &filename){
    emit this->motionFileChanged(filename);
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open motion file(.csv)",QString(), "motion (*.csv);;any (*.*)");
    if(filename.size() != 0)
    {
        emit this->motionFileChanged(filename);
    }
}

void MainWindow::configure()
{
}

void MainWindow::playstopButtonClicked()
{
    if(this->viewerwidget->isPlaying()){
        this->viewerwidget->stop();
    }
    else{
        this->viewerwidget->play();
    }
}

void MainWindow::setPlayStopButtonState(bool isplaying){
    if(isplaying){
        this->play_stop->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
    }
    else{
        this->play_stop->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void MainWindow::backButtonClicked()
{
    this->viewerwidget->setCurrentFrame(1);
}

void MainWindow::nextButtonClicked()
{
    this->viewerwidget->setCurrentFrame(-1);
}

void MainWindow::onMotionFileChanged(const QString &filename){
    this->filename->setText(filename);
}
