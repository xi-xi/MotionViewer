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

    void drawBoxGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix);

    void setWidth(float val);
    void setHeight(float val);
    void setDepth(float val);
    void setWHD(float w, float h, float d);
    void setModelMatrix(const QMatrix4x4& matrix);
    float getWidth()const;
    float getHeight()const;
    float getDepth()const;
    const QMatrix4x4& getModelMatrix()const;

private:
    void initBoxGeometry();
    void updateVertices();

    float width;
    float height;
    float depth;

    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexbuf;
    QMatrix4x4 model_matrix;
};

#endif // BOXGEOMETRYENGINE_H
