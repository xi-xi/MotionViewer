#include "motionviewerwidget.h"
#include <QtGlobal>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include "motion.h"
#include "motiongeometryengine.h"
#include "planegeometryengine.h"

MotionViewerWidget::MotionViewerWidget(QWidget* parent):
    QOpenGLWidget(parent),
    playing(false),
    current_frame(1),
    geometries(0)
{
    this->motion = new Motion(this);
    this->motion_loaded = false;
    this->connect(this, SIGNAL(motionChanged()), this, SLOT(update()));
    this->timer = new QTimer(this);
    this->timer->setInterval(this->FRAME_UPDATE_MSEC);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateCurrentFrame()));
    connect(this, SIGNAL(motionChanged()), this, SLOT(onMotionChanged()));
    connect(this, SIGNAL(motionChanged()), this, SLOT(updateMotionProperties()));
    connect(this, SIGNAL(currentFrameChanged(int)), this, SLOT(update()));
}

MotionViewerWidget::~MotionViewerWidget()
{
    this->timer->stop();
    this->makeCurrent();
    delete this->geometries;
    delete this->plane;
    this->doneCurrent();
}

void MotionViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    this->initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    this->geometries = new MotionGeometryEngine();
    this->plane = new PlaneGeometryEngine(4.0);
}

void MotionViewerWidget::initShaders()
{
    if(!this->program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    {
        qDebug() << "MotionViewerWidget::initShaders() vertex shader add failed.";
        this->close();
    }
    if(!this->program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
    {
        qDebug() << "MotionViewerWidget::initShaders() fragment shader add failed.";
        this->close();
    }
    if(!this->program.link())
    {
        qDebug() << "MotionViewerWidget::initShaders() link failed.";
        this->close();
    }
    if(!this->program.bind())
    {
        qDebug() << "MotionViewerWidget::initShaders() bind failed.";
        this->close();
    }
}

void MotionViewerWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar =10000.0, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void MotionViewerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    QMatrix4x4 matrix;
    matrix.translate(0.0, -750.0, -2500.0);
    this->plane->draw(&this->program, projection * matrix);
    if(!this->motion_loaded)
        return;
    //this->program.setUniformValue("texture", 0);
    this->geometries->drawMotionGeometry(
                &this->program,
                projection * matrix,
                this->motion->at(this->current_frame)
                );
}

void MotionViewerWidget::openMotionFile(const QString &filename){
    this->motion_loaded = this->motion->open(filename);
    emit this->motionChanged();
}

void MotionViewerWidget::updateMotionProperties()
{
    if(this->motion_loaded){
        this->fps = this->motion->getProperty("DataRate");
        this->max_frame = this->motion->getProperty("NumFrames");
    }
}

void MotionViewerWidget::onMotionChanged()
{

}

bool MotionViewerWidget::isPlaying()const{
    return this->playing;
}

void MotionViewerWidget::play()
{
    this->timer->start();
    this->playing = true;
    this->timer_tick_count = this->current_frame / this->fps * 1000.0 / this->FRAME_UPDATE_MSEC;
}

void MotionViewerWidget::stop()
{
    this->timer->stop();
    this->playing = false;
    this->timer_tick_count = this->current_frame / this->fps * 1000.0 / this->FRAME_UPDATE_MSEC;
}

void MotionViewerWidget::updateCurrentFrame()
{
    if(this->current_frame >= this->max_frame){
        this->current_frame = this->max_frame;
    }
    else{
        this->timer_tick_count++;
        this->current_frame = this->timer_tick_count * this->FRAME_UPDATE_MSEC / 1000.0 * this->fps;
        if(this->current_frame >= this->max_frame){
            this->current_frame = this->max_frame;
        }
        emit this->currentFrameChanged(this->current_frame);
    }
}

void MotionViewerWidget::setCurrentFrame(int frame){
    this->current_frame = frame;
    if(frame <= 0){
        this->current_frame = this->max_frame;
    }
    else if(this->current_frame >= this->max_frame){
        this->current_frame = this->max_frame;
    }
    this->timer_tick_count = this->current_frame / this->fps * 1000.0 / this->FRAME_UPDATE_MSEC;
    emit this->currentFrameChanged(this->current_frame);
}
