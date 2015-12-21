#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>
#include <QBasicTimer>
#include "pose.h"
class QMatrix4x4;
class QOpenGLShaderProgram;
class BoxGeometryEngine;

class MotionGeometryEngine
{
public:
    MotionGeometryEngine();
    virtual ~MotionGeometryEngine();

    void drawMotionGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix, const Pose* pose);

private:
    QList<BoxGeometryEngine*> boxes;
    QBasicTimer timer;
};

#endif // MOTIONGEOMETRYENGINE_H
