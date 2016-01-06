#include "motion.h"

#include <QFile>
#include <QTextStream>

Motion::Motion(QObject *parent) : QObject(parent),
    fps_(.0), maxFrame_(0)
{

}

Motion::~Motion()
{
    qDeleteAll(this->poses);
}

const Pose* Motion::at(int frame) const{
    return this->poses[frame];
}

Pose *Motion::at(int frame){
    return this->poses[frame];
}

float Motion::fps()const{
    return this->fps_;
}

void Motion::setFps(float fps){
    this->fps_ = fps;
}

int Motion::maxFlame()const{
    return this->maxFrame_;
}

void Motion::setMaxFlame(int max_frame){
    this->maxFrame_ = max_frame;
}

const QStringList& Motion::markers()const{
    return this->markers_;
}

void Motion::setMarkers(const QStringList &markers){
    this->markers_ = markers;
}

void Motion::set(int frame, Pose *pose){
    this->poses[frame] = pose;
}
