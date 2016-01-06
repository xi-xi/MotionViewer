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
    ~Motion();
    const Pose* operator[](int frame)const{
        return this->at(frame);
    }
    Pose* operator[](int frame){
        return this->at(frame);
    }

    const Pose* at(int frame)const;
    Pose* at(int frame);
    void set(int frame, Pose* pose);
    float fps()const;
    void setFps(float fps);
    int maxFlame()const;
    void setMaxFlame(int max_frame);
    const QStringList& markers()const;
    void setMarkers(const QStringList& markers);

signals:

public slots:

private:
    QMap<int, Pose*> poses;
    QStringList markers_;
    float fps_;
    int maxFrame_;
};

#endif // MOTION_H
