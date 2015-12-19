#include "motionviewerwidget.h"
#include <QtGlobal>
#include <QDebug>

MotionViewerWidget::MotionViewerWidget(QWidget* parent):
    QOpenGLWidget(parent),
    texture(0)
{
}

MotionViewerWidget::~MotionViewerWidget()
{
    this->makeCurrent();
    delete this->texture;
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
    this->texture = new QOpenGLTexture(QImage(":/texture.png").mirrored());
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
    this->texture->bind();
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    this->program.setUniformValue("mvp_matrix", projection * matrix);
    this->program.setUniformValue("texture", 0);
}
