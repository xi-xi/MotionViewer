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
    scale(scale),
    indexbuf(QOpenGLBuffer::IndexBuffer),
    texture(0)
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
    this->texture = new QOpenGLTexture(QImage(":/tile.png").mirrored());
    this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
    this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void PlaneGeometryEngine::initGeometry()
{
    VertexData vertices[] = {
        {QVector3D( 1.0f, 0.0f,  1.0f), QVector2D(0.0f, 0.0f)},
        {QVector3D( 1.0f, 0.0f, -1.0f), QVector2D(scale / 500.0, 0.0f)},
        {QVector3D(-1.0f, 0.0f,  1.0f), QVector2D(0.0f, scale / 500.0)},
        {QVector3D( -1.0f, 0.0f,  -1.0f), QVector2D(scale / 500.0, scale / 500.0)}
    };
    this->arraybuf.bind();
    this->arraybuf.allocate(vertices, 4 * sizeof(VertexData));
    GLushort indices[] = {
        0, 1, 2, 3
    };
    this->indexbuf.bind();
    this->indexbuf.allocate(indices, 4 * sizeof(GLushort));
}

void PlaneGeometryEngine::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &vp_matrix)
{
    this->texture->bind();
    QMatrix4x4 scale;
    scale.scale(this->scale * 0.5, 1.0, this->scale * 0.5);
    program->setUniformValue("mvp_matrix", vp_matrix * scale);
    this->arraybuf.bind();
    this->indexbuf.bind();
    quintptr offset = 0;
    int vertexlocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexlocation);
    program->setAttributeBuffer(vertexlocation, GL_FLOAT, offset, 3,sizeof(VertexData));
    offset += sizeof(QVector3D);
    int texcoordlocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordlocation);
    program->setAttributeBuffer(texcoordlocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
}
