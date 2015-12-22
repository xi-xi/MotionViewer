#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>
#include <QBasicTimer>
#include "pose.h"
class Motion;
class QMatrix4x4;
class QOpenGLShaderProgram;
class BoxGeometryEngine;
class DrawConfigure;

class MotionGeometryEngine
{
public:
    MotionGeometryEngine();
    virtual ~MotionGeometryEngine();

    void drawMotionGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix, const Pose* pose);

private:
    QList<BoxGeometryEngine*> boxes;
    QBasicTimer timer;
    DrawConfigure* config;
};

#endif // MOTIONGEOMETRYENGINE_H
