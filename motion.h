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
    const Pose* operator[](int frame)const{
        return this->at(frame);
    }
    Pose* operator[](int frame){
        return this->at(frame);
    }

    const Pose* at(int frame)const;
    Pose* at(int frame);
    float fps()const;
    void setFps(float fps);
    int maxFlame()const;
    void setMaxFlame(int max_frame);
    const QStringList& markers()const;
    void setMarkers(const QStringList& markers);

signals:

public slots:

private:
    bool build(QTextStream& stream);
    bool buildProperty(const QString& name, const QString& value);
    bool buildPose(const QString& line, Pose* pose);

    QMap<int, Pose*> poses;
    QStringList markers;
    float fps;
    int maxFrame;
};

#endif // MOTION_H
