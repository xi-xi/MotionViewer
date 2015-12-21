#include "motiongeometryengine.h"

#include <QMatrix4x4>
#include "boxgeometryengine.h"

MotionGeometryEngine::MotionGeometryEngine()
{
    this->boxes.append(new BoxGeometryEngine(1.0, 2.0, 2.0));
    this->boxes.append(new BoxGeometryEngine(2.0, 1.0, 2.0));
}

MotionGeometryEngine::~MotionGeometryEngine()
{
    qDeleteAll(this->boxes);
}

void MotionGeometryEngine::drawMotionGeometry(QOpenGLShaderProgram *program, const QMatrix4x4& vp_matrix, const Pose* pose)
{
    for(auto box : this->boxes){
        box->drawBoxGeometry(program, vp_matrix);
    }
}
