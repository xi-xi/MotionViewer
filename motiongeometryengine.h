#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>
#include <QBasicTimer>
class QOpenGLShaderProgram;
class BoxGeometryEngine;

class MotionGeometryEngine
{
public:
    MotionGeometryEngine();
    virtual ~MotionGeometryEngine();

    void drawMotionGeometry(QOpenGLShaderProgram* program);

private:
    QList<BoxGeometryEngine*> boxes;
    QBasicTimer timer;
};

#endif // MOTIONGEOMETRYENGINE_H
