#include "boxgeometryengine.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

BoxGeometryEngine::BoxGeometryEngine():
    width(2.0),
    height(2.0),
    depth(2.0),
    indexbuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    this->arraybuf.create();
    this->arraybuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    this->indexbuf.create();
    this->initBoxGeometry();
}

BoxGeometryEngine::BoxGeometryEngine(float w, float h, float d):
    width(w),
    height(h),
    depth(d),
    indexbuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    this->arraybuf.create();
    this->arraybuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    this->indexbuf.create();
    this->initBoxGeometry();
}

BoxGeometryEngine::~BoxGeometryEngine()
{
    this->arraybuf.destroy();
    this->indexbuf.destroy();
}

void BoxGeometryEngine::initBoxGeometry()
{
    VertexData vertices[] = {
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };
    this->arraybuf.bind();
    this->arraybuf.allocate(vertices, 24 * sizeof(VertexData));
    this->updateModelMatrix();
    GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };
    this->indexbuf.bind();
    this->indexbuf.allocate(indices, 34 * sizeof(GLushort));
}

void BoxGeometryEngine::updateModelMatrix()
{
    const float x = this->width / 2.0;
    const float y = this->height / 2.0;
    const float z = this->depth / 2.0;
    this->model_matrix = QMatrix4x4();
    this->model_matrix.scale(x, y, z);
}

void BoxGeometryEngine::drawBoxGeometry(QOpenGLShaderProgram *program, const QMatrix4x4 &vp_matrix)
{
    program->setUniformValue("mvp_matrix", vp_matrix * this->model_matrix);
    this->arraybuf.bind();
    this->indexbuf.bind();
    quintptr offset = 0;
    int vertexlocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexlocation);
    program->setAttributeBuffer(vertexlocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    offset += sizeof(QVector3D);
    int texcoordlocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordlocation);
    program->setAttributeBuffer(texcoordlocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

void BoxGeometryEngine::setWidth(float val)
{
    this->width = val;
    this->updateModelMatrix();
}

void BoxGeometryEngine::setHeight(float val){
    this->height = val;
    this->updateModelMatrix();
}

void BoxGeometryEngine::setDepth(float val){
    this->depth = val;
    this->updateModelMatrix();
}

void BoxGeometryEngine::setWHD(float w, float h, float d){
    this->width = w;
    this->height = h;
    this->depth = d;
    this->updateModelMatrix();
}

float BoxGeometryEngine::getWidth()const{
    return this->width;
}

float BoxGeometryEngine::getHeight()const{
    return this->height;
}

float BoxGeometryEngine::getDepth()const{
    return this->depth;
}

void BoxGeometryEngine::setModelMatrix(const QMatrix4x4 &matrix){
    this->model_matrix = matrix;
}

const QMatrix4x4& BoxGeometryEngine::getModelMatrix() const{
    return this->model_matrix;
}
