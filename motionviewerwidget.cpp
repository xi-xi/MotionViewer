#include "motionviewerwidget.h"
#include <QtGlobal>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <chrono>

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
    this->plane = new PlaneGeometryEngine(4000.0);
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
    this->aspect = qreal(w) / qreal(h ? h : 1);
    this->updatePerspective();
}

void MotionViewerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    QMatrix4x4 matrix;
    matrix.translate(-1.0 * this->camera_translate);
    matrix.rotate(-1.0 * this->camera_angle, QVector3D(.0, 1.0, .0));
    this->program.setUniformValue("texture", 0);
    this->plane->draw(&this->program, projection * matrix);
    if(!this->motion_loaded)
        return;
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
    this->start_time = Time::now();
    emit this->playStateChanged(this->isPlaying());
}

void MotionViewerWidget::stop()
{
    this->timer->stop();
    this->playing = false;
    this->start_frame = this->current_frame;
    emit this->playStateChanged(this->isPlaying());
}

void MotionViewerWidget::updateCurrentFrame()
{
    namespace sc = std::chrono;
    if(this->current_frame >= this->max_frame){
        this->current_frame = this->max_frame;
    }
    else{
        auto now = sc::system_clock::now();
        this->current_frame = this->start_frame + sc::duration_cast<std::chrono::milliseconds>(now - this->start_time).count() / 1000.0 * this->fps;
        if(this->current_frame >= this->max_frame){
            this->current_frame = this->max_frame;
            this->stop();
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
    this->start_frame = frame;
    this->start_time = std::chrono::system_clock::now();
    emit this->currentFrameChanged(this->current_frame);
}

void MotionViewerWidget::mousePressEvent(QMouseEvent *event){
    this->mouseclicked_position = event->pos();
}

void MotionViewerWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        QPoint vec = event->pos() - this->mouseclicked_position;
        this->camera_translate.setX(this->camera_translate.x() + vec.x());
        this->camera_translate.setY(this->camera_translate.y() + vec.y());
        this->updatePerspective();
        this->mouseclicked_position = event->pos();
    }
}

void MotionViewerWidget::wheelEvent(QWheelEvent *event){
    if(event->orientation() == Qt::Horizontal
            || (event->orientation() == Qt::Vertical && event->modifiers() & Qt::ShiftModifier)){
        this->camera_angle += event->delta() / 100.0;
        if(!this->isPlaying())
            this->update();
    }
    else{
        this->fov *= pow(1.1 , event->delta() / 100.0);
        if(this->fov > FOV_UPPER_LIMIT){
            this->fov = FOV_UPPER_LIMIT;
        }
        else if(this->fov < FOV_DOWN_LIMIT){
            this->fov = FOV_DOWN_LIMIT;
        }
        this->updatePerspective();
    }
}

void MotionViewerWidget::updatePerspective()
{
    projection.setToIdentity();
    projection.perspective(this->fov, this->aspect, this->zNear, this->zFar);
    if(!this->isPlaying())
        this->update();
}
