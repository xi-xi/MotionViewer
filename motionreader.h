#ifndef MOTIONREADER_H
#define MOTIONREADER_H

#include <QObject>

class MotionReader : public QObject
{
    Q_OBJECT
public:
    explicit MotionReader(QObject *parent = 0);

signals:

public slots:
};

#endif // MOTIONREADER_H