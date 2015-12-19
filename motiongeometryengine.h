#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class MotionGeometryEngine : protected QOpenGLFunctions
{
public:
    MotionGeometryEngine();
    virtual ~MotionGeometryEngine();

    void drawMotionGeometry(QOpenGLShaderProgram* program);

private:
    void initMotionGeometry();

    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexBuf;
};

#endif // MOTIONGEOMETRYENGINE_H
