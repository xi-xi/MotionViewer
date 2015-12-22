#include "motiongeometryengine.h"

#include <QMatrix4x4>
#include "boxgeometryengine.h"
#include "motion.h"
#include "drawconfigure.h"

MotionGeometryEngine::MotionGeometryEngine()
{
    this->config = DrawConfigure::defaultConfigure();
}

MotionGeometryEngine::~MotionGeometryEngine()
{
    delete this->config;
    qDeleteAll(this->boxes);
}

void MotionGeometryEngine::drawMotionGeometry(QOpenGLShaderProgram *program, const QMatrix4x4& vp_matrix, const Pose* pose)
{
    static float angle = 0.0;
    for(auto box : this->boxes){
        QMatrix4x4 model = box->getModelMatrix();
        QMatrix4x4 rotate;
        rotate.rotate(angle, QVector3D(0, 0, 1));
        box->setModelMatrix(rotate * model);
        box->drawBoxGeometry(program, vp_matrix);
        angle+=0.001;
    }
}
