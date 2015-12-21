#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>
#include <QBasicTimer>
class QMatrix4x4;
class QOpenGLShaderProgram;
class BoxGeometryEngine;

class MotionGeometryEngine
{
public:
    MotionGeometryEngine();
    virtual ~MotionGeometryEngine();

    void drawMotionGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix);

private:
    QList<BoxGeometryEngine*> boxes;
    QBasicTimer timer;
};

#endif // MOTIONGEOMETRYENGINE_H
