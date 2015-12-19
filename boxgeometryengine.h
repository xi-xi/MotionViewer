#ifndef BOXGEOMETRYENGINE_H
#define BOXGEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>

class BoxGeometryEngine : protected QOpenGLFunctions
{
public:
    BoxGeometryEngine();
    BoxGeometryEngine(float w, float h, float d);
    virtual ~BoxGeometryEngine();

    void drawBoxGeometry(QOpenGLShaderProgram* program);

private:
    void initBoxGeometry();
    void updateVertices();

    float width;
    float height;
    float depth;

    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexbuf;
};

#endif // BOXGEOMETRYENGINE_H
