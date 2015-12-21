#ifndef POSE_H
#define POSE_H

#include <QObject>
#include <QMap>
#include <QVector3D>

class Pose : public QObject
{
    Q_OBJECT
public:
    explicit Pose(QObject *parent = 0);
    void addJointData(const QString& name, const QVector3D& position);
    void addJointData(const QString &name, float x, float y, float z);
    const QVector3D& operator[](const QString& name)const{
        return this->at(name);
    }
    const QVector3D& at(const QString& name)const;
    bool contain(const QString& name)const;
    QList<QString> getJointsName()const;

signals:

public slots:

private:
    QMap<QString, QVector3D> joint_positions;
};

#endif // POSE_H
