#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QStyle>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QKeyEvent>
#include <QSlider>
#include <QSpinBox>

#include "motion.h"
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

    this->slider = new QSlider(Qt::Horizontal, this);
    this->box = new QSpinBox(this);
    this->maxframe_label = new QLabel(this);

    this->initUI();
    this->initLogic();

    this->setWindowTitle("Motion Viewer");
    this->setAcceptDrops(true);
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
    this->operationbar->addWidget(this->slider);
    this->slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    this->operationbar->addWidget(this->box);
    this->operationbar->addWidget(this->maxframe_label);
}

void MainWindow::initLogic()
{
    this->connect(this, SIGNAL(motionFileChanged(QString)), this->viewerwidget, SLOT(openMotionFile(QString)));
    this->connect(this, SIGNAL(motionFileChanged(QString)), this, SLOT(onMotionFileChanged(QString)));
    this->connect(this->openaction, SIGNAL(triggered()), this, SLOT(openFile()));
    this->connect(this->configureaction, SIGNAL(triggered()), this, SLOT(configure()));
    this->connect(this->closeaction, SIGNAL(triggered()), this, SLOT(close()));
    this->connect(this->back, SIGNAL(triggered()), this, SLOT(backButtonClicked()));
    this->connect(this->play_stop, SIGNAL(triggered()), this, SLOT(playstopButtonClicked()));
    this->connect(this->next, SIGNAL(triggered()), this, SLOT(nextButtonClicked()));
    this->connect(this->viewerwidget, SIGNAL(playStateChanged(bool)), this, SLOT(setPlayStopButtonState(bool)));
    this->connect(this->viewerwidget, SIGNAL(currentFrameChanged(int)), this->slider, SLOT(setValue(int)));
    this->connect(this->viewerwidget, SIGNAL(currentFrameChanged(int)), this->box, SLOT(setValue(int)));
    this->connect(this->slider, SIGNAL(valueChanged(int)), this->viewerwidget, SLOT(setCurrentFrame(int)));
    this->connect(this->box, SIGNAL(valueChanged(int)), this->viewerwidget, SLOT(setCurrentFrame(int)));
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
    this->setWindowTitle("Motion Viewer  " + filename);
    this->slider->setMinimum(1);
    this->slider->setMaximum(this->viewerwidget->getMotion()->maxFlame());
    this->box->setMinimum(1);
    this->box->setMaximum(this->viewerwidget->getMotion()->maxFlame());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event){
    if(event->mimeData()->hasUrls()){
        QStringList pathlist;
        QList<QUrl> urlList = event->mimeData()->urls();
        for(int i = 0;i<urlList.size();++i){
            pathlist.append(urlList.at(i).toLocalFile());
        }
        this->openMotionFile(pathlist.at(0));
        for(int i = 1;i<pathlist.size();++i){
            MainWindow* w = new MainWindow();
            w->show();
            w->resize(this->size());
            w->openMotionFile(pathlist.at(i));
        }
        event->acceptProposedAction();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        this->playstopButtonClicked();
    }
}
