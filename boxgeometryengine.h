#ifndef BOXGEOMETRYENGINE_H
#define BOXGEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class BoxGeometryEngine : protected QOpenGLFunctions
{
public:
    BoxGeometryEngine();
    BoxGeometryEngine(float w, float h, float d);
    virtual ~BoxGeometryEngine();

    void drawBoxGeometry(QOpenGLShaderProgram* program);

    void setWidth(float val);
    void setHeight(float val);
    void setDepth(float val);
    void setWHD(float w, float h, float d);
    float getWidth()const;
    float getHeight()const;
    float getDepth()const;

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
