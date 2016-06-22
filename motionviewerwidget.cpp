#include "motionviewerwidget.h"
#include <QtGlobal>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <chrono>
#include <QDebug>

#include "motion.h"
#include "motionbuilder.h"
#include "motiongeometryengine.h"
#include "planegeometryengine.h"

MotionViewerWidget::MotionViewerWidget(QWidget* parent):
    QOpenGLWidget(parent),
    playing(false),
    current_frame(1),
    max_frame(1),
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
    this->updateCameraMatrix();
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
    this->program.setUniformValue("texture", 0);
    this->plane->draw(&this->program, projection * this->camera_matrix);
    if(!this->motion_loaded)
        return;
    this->geometries->drawMotionGeometry(
                &this->program,
                projection * this->camera_matrix,
                this->motion->at(this->current_frame)
                );
}

void MotionViewerWidget::openMotionFile(const QString &filename){
    this->motion = MotionBuilder::open(filename, this);
    this->motion_loaded = this->motion != nullptr;
    emit this->motionChanged();
}

void MotionViewerWidget::updateMotionProperties()
{
    if(this->motion_loaded){
        this->fps = this->motion->fps();
        this->max_frame = this->motion->maxFlame();
    }
}

void MotionViewerWidget::onMotionChanged()
{
    this->stop();
    this->setCurrentFrame(1);
}

bool MotionViewerWidget::isPlaying()const{
    return this->playing;
}

void MotionViewerWidget::play()
{
    if(this->current_frame >= this->max_frame){
        this->setCurrentFrame(1);
    }
    this->timer->start();
    this->playing = true;
    this->start_time = Time::now();
    emit this->playStateChanged(this->isPlaying());
}

void MotionViewerWidget::stop()
{
    if(!this->isPlaying()){
        return;
    }
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
        this->stop();
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
    if(this->current_frame == frame){
        return;
    }
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
    if(event->buttons() & Qt::LeftButton){
        this->mouse_left_clicked_position = event->pos();
    }
    if(event->buttons() & Qt::RightButton){
        this->mouse_right_clicked_position = event->pos();
    }
}

void MotionViewerWidget::mouseMoveEvent(QMouseEvent *event){
    const double pi = 3.1415;
    if(event->buttons() & Qt::LeftButton){
        QPoint vec = event->pos() - this->mouse_left_clicked_position;
        float coef = 2 * 4 * pi / this->width();
        this->addCameraHorizontalAngle(-vec.x() * coef);
        this->addCameraVerticalAngle(vec.y() * coef);
        this->mouse_left_clicked_position = event->pos();
    }
    if(event->buttons() & Qt::RightButton){
        QPoint vec = event->pos() - this->mouse_right_clicked_position;
        QVector3D right_move = this->camera_right * vec.x();
        QVector3D up_move = this->camera_up * vec.y();
        this->camera_center += right_move + up_move;
        this->updateCameraMatrix();
        this->mouse_right_clicked_position = event->pos();
    }
}

void MotionViewerWidget::wheelEvent(QWheelEvent *event){
    if(event->orientation() == Qt::Horizontal
            || (event->orientation() == Qt::Vertical && event->modifiers() & Qt::ShiftModifier)){
        this->addCameraHorizontalAngle(event->delta() / 100.0);
    }
    else{
        this->camera_radius += event->delta() / 5.0;
        if(this->camera_radius > this->maximum_camera_radius){
            this->camera_radius = this->maximum_camera_radius;
        }
        else if(this->camera_radius < this->minimum_camera_radius){
            this->camera_radius = this->minimum_camera_radius;
        }
        this->updateCameraMatrix();
    }
}

void MotionViewerWidget::updatePerspective()
{
    projection.setToIdentity();
    projection.perspective(this->fov, this->aspect, this->zNear, this->zFar);
    if(!this->isPlaying())
        this->update();
}

const Motion* MotionViewerWidget::getMotion()const{
    return this->motion;
}

bool MotionViewerWidget::motionLoaded()const{
    return this->motion_loaded;
}

void MotionViewerWidget::updateCameraMatrix()
{
    qreal hsin = qSin(qDegreesToRadians(this->camera_h_angle));
    qreal hcos = qCos(qDegreesToRadians(this->camera_h_angle));
    qreal vsin = qSin(qDegreesToRadians(this->camera_v_angle));
    qreal vcos = qCos(qDegreesToRadians(this->camera_v_angle));
    QVector3D camera_position = QVector3D(
        this->camera_radius * vcos * hcos,
        this->camera_radius * vcos * hsin,
        this->camera_radius * vsin
    );
    this->camera_matrix.setToIdentity();
    this->camera_matrix.lookAt(camera_position + this->camera_center, this->camera_center, this->camera_up);
    qDebug() << this->camera_h_angle << this->camera_v_angle;
    qDebug() << this->camera_center;
    qDebug() << camera_position;
    if(!this->isPlaying()){
        this->update();
    }
}

void MotionViewerWidget::addCameraHorizontalAngle(qreal angle){
    this->camera_h_angle += angle;
    QMatrix4x4 rot;
    rot.rotate(angle, QVector3D(0, 1, 0));
    this->camera_up = rot.mapVector(this->camera_up);
    this->camera_right = rot.mapVector(this->camera_right);
    this->updateCameraMatrix();
}

void MotionViewerWidget::addCameraVerticalAngle(qreal angle){
    this->camera_v_angle += angle;
    QMatrix4x4 rot;
    rot.rotate(-angle, this->camera_right);
    this->camera_up = rot.mapVector(this->camera_up);
//    this->camera_up = QVector3D::crossProduct(this->camera_position, this->camera_right).normalized();
    this->updateCameraMatrix();
}
