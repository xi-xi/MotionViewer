#ifndef MOTION_H
#define MOTION_H

#include <QObject>

class Motion : public QObject
{
    Q_OBJECT
public:
    explicit Motion(QObject *parent = 0);

signals:

public slots:
};

#endif // MOTION_H