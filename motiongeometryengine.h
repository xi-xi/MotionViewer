#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>

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
};

#endif // MOTIONGEOMETRYENGINE_H
