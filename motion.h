#ifndef MOTION_H
#define MOTION_H

#include <QObject>
#include <QMap>
#include <QTextStream>

#include "pose.h"

class Motion : public QObject
{
    Q_OBJECT
public:
    explicit Motion(QObject *parent = 0);
    Motion(const QString& filename, QObject* parent = 0);
    ~Motion();
    bool open(const QString& filename);
    bool containsProperty(const QString& name)const;
    float getProperty(const QString& name)const;
    const Pose* operator[](int frame)const{
        return this->at(frame);
    }
    Pose* operator[](int frame){
        return this->at(frame);
    }

    const Pose* at(int frame)const;
    Pose* at(int frame);

signals:

public slots:

private:
    bool build(QTextStream& stream);
    bool buildProperty(const QString& name, const QString& value);
    bool buildPose(const QString& line, Pose* pose);

    QMap<int, Pose*> poses;
    QStringList markers;
    QMap<QString, float> properties;
};

#endif // MOTION_H
