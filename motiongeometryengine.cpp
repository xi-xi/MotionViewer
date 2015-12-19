#include "motiongeometryengine.h"

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

void MotionGeometryEngine::drawMotionGeometry(QOpenGLShaderProgram *program)
{
    for(auto box : this->boxes){
        box->drawBoxGeometry(program);
    }
}
