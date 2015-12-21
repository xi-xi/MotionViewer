#include "pose.h"

Pose::Pose(QObject *parent) : QObject(parent)
{

}

void Pose::addJointData(const QString &name, const QVector3D &position)
{
    this->joint_positions.insert(name, position);
}

void Pose::addJointData(const QString &name, float x, float y, float z)
{
    this->addJointData(name, QVector3D(x, y, z));
}

const QVector3D& Pose::at(const QString &name) const
{
    return this->joint_positions[name];
}

bool Pose::contain(const QString &name) const
{
    return this->joint_positions.contains(name);
}

QList<QString> Pose::getJointsName()const{
    return this->joint_positions.keys();
}
