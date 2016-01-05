#ifndef MOTIONBUILDER_H
#define MOTIONBUILDER_H

#include <QObject>

class QString;

class Motion;
class Pose;

class MotionBuilder : public QObject
{
    Q_OBJECT
public:
    explicit MotionBuilder(QObject *parent = 0);
    static Motion* open(const QString& filename);

private:
    static Motion* buildFromCSV(const QString& filename);
    static Motion* buildFromTRC(const QString& filename);
    static Motion* buildFromTS(const QString& filename);

    static bool buildPoseFromCSV(const QString& line, Motion* motion);
};

#endif // MOTIONBUILDER_H
