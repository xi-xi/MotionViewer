#ifndef POSE_H
#define POSE_H

#include <QObject>

class Pose : public QObject
{
    Q_OBJECT
public:
    explicit Pose(QObject *parent = 0);

signals:

public slots:
};

#endif // POSE_H