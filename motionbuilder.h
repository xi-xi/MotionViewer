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
    static Motion* open(const QString& filename, QObject* parent=0);

private:
    explicit MotionBuilder(QObject *parent = 0);
    static Motion* buildFromCSV(const QString& filename, QObject* parent=0);
    static Motion* buildFromTRC(const QString& filename, QObject* parent=0);
    static Motion* buildFromTS(const QString& filename, QObject* parent=0);

    static bool buildPoseFromCSV(const QStringList& cells, Motion* motion);
    static bool buildPoseFromTRC(const QStringList& cells, Motion* motion);
    static bool buildPoseFromTS(const QStringList& cells, Motion* motion);
};

#endif // MOTIONBUILDER_H
