#include "motionviewerwidget.h"
#include <QtGlobal>
#include <QDebug>

#include "motion.h"
#include "motiongeometryengine.h"

MotionViewerWidget::MotionViewerWidget(QWidget* parent):
    QOpenGLWidget(parent),
    playing(false),
    current_frame(1),
    geometries(0),
    texture(0)
{
    this->motion = new Motion(this);
    this->motion_loaded = false;
    this->connect(this, SIGNAL(motionChanged()), this, SLOT(update()));
}

MotionViewerWidget::~MotionViewerWidget()
{
    this->makeCurrent();
    delete this->texture;
    delete this->geometries;
    this->doneCurrent();
}

void MotionViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    this->initShaders();
    this->initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    this->geometries = new MotionGeometryEngine();
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

void MotionViewerWidget::initTextures()
{
    this->texture = new QOpenGLTexture(QImage(":/blackboard.png").mirrored());
    this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
    this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void MotionViewerWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar =7.0, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void MotionViewerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    if(!this->motion_loaded)
        return;
    this->texture->bind();
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    this->program.setUniformValue("texture", 0);
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

bool MotionViewerWidget::isPlaying()const{
    return this->playing;
}

void MotionViewerWidget::play()
{
    this->playing = !this->playing;
}

void MotionViewerWidget::stop()
{
    this->playing = !this->playing;
}
