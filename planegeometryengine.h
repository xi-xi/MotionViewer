#ifndef PLANEGEOMETRYENGINE_H
#define PLANEGEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class QOpenGLShaderProgram;
class QOpenGLTexture;

class PlaneGeometryEngine : protected QOpenGLFunctions
{
public:
    PlaneGeometryEngine(float scale = 1.0);
    ~PlaneGeometryEngine();

    void draw(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix);
private:
    void initTexture();
    void initGeometry();

    float scale;
    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexbuf;
    QOpenGLTexture *texture;
};

#endif // PLANEGEOMETRYENGINE_H
