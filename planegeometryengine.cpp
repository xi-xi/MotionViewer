#include "planegeometryengine.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QVector2D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

PlaneGeometryEngine::PlaneGeometryEngine(float scale):
    texture(0),
    indexbuf(QOpenGLBuffer::IndexBuffer),
    scale(scale)
{
    initializeOpenGLFunctions();
    this->arraybuf.create();
    this->indexbuf.create();
    this->initTexture();
    this->initGeometry();
}

PlaneGeometryEngine::~PlaneGeometryEngine()
{
    this->arraybuf.destroy();
    this->indexbuf.destroy();
    delete this->texture;
}

void PlaneGeometryEngine::initTexture()
{
}

void PlaneGeometryEngine::initGeometry()
{
    VertexData vertices[] = {
        {QVector3D(-1.0f, 0.0f,  1.0f), QVector2D(0.0f, 0.0f)},
        {QVector3D( 1.0f, 0.0f,  1.0f), QVector2D(0.33f, 0.0f)},
        {QVector3D(-1.0f,  0.0f,  1.0f), QVector2D(0.0f, 0.5f)},
        {QVector3D( 1.0f,  0.0f,  1.0f), QVector2D(0.33f, 0.5f)}
    };
}
