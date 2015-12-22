#include "motiongeometryengine.h"

#include <QMatrix4x4>
#include <QList>
#include <QPair>
#include <QtMath>
#include <QOpenGLTexture>
#include "boxgeometryengine.h"
#include "motion.h"
#include "drawconfigure.h"

MotionGeometryEngine::MotionGeometryEngine()
{
    this->initTexture();
    this->config = DrawConfigure::defaultConfigure();
    this->initBoxes();
}

MotionGeometryEngine::~MotionGeometryEngine()
{
    delete this->texture;
    delete this->config;
    qDeleteAll(this->boxes);
}

void MotionGeometryEngine::initTexture(){
    this->texture = new QOpenGLTexture(QImage(":/blackboard.png").mirrored());
    this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
    this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void MotionGeometryEngine::initBoxes()
{
    for(int i = 0; i < this->config->getConnectJoints().size();++i){
        this->boxes.append(new BoxGeometryEngine(1.0, 1.0, 1.0));
    }
}

void MotionGeometryEngine::drawMotionGeometry(QOpenGLShaderProgram *program, const QMatrix4x4& vp_matrix, const Pose* pose)
{
    this->texture->bind();
    for(int i = 0; i < this->config->getConnectJoints().size() ; ++i){
        if(!pose->contain(this->config->getConnectJoints()[i].first)
                || !pose->contain(this->config->getConnectJoints()[i].second)){
            continue;
        }
        QVector3D first_position = pose->at(this->config->getConnectJoints()[i].first);
        QVector3D second_position = pose->at(this->config->getConnectJoints()[i].second);
        QMatrix4x4 model = this->calcModelMatrix(first_position, second_position);
        this->boxes[i]->setModelMatrix(model);
        this->boxes[i]->drawBoxGeometry(program, vp_matrix);
    }
}

QMatrix4x4 MotionGeometryEngine::calcModelMatrix(const QVector3D &first, const QVector3D &second)
{
    float length = (first - second).length();
    QMatrix4x4 scale_matrix;
    scale_matrix.scale(config->box_size.x() * 0.5, length * 0.5, config->box_size.z() * 0.5);

    auto dif = (second - first).normalized();
    auto axis = QVector3D::crossProduct(QVector3D(0, 1, 0), dif);
    auto angle =qRadiansToDegrees(acos(QVector3D::dotProduct(QVector3D(0, 1, 0), dif)));
    QMatrix4x4 rotate_matrix;
    rotate_matrix.rotate(angle, axis);

    QMatrix4x4 trans_matrix;
    auto trans = (first + second) / 2.0;
    trans_matrix.translate(trans);
    return trans_matrix * rotate_matrix * scale_matrix;
}
