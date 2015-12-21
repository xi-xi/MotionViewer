#include "motion.h"

#include <QFile>
#include <QTextStream>

Motion::Motion(QObject *parent) : QObject(parent)
{

}

Motion::Motion(const QString &filename, QObject *parent) :
    QObject(parent)
{
    this->open(filename);
}

Motion::~Motion()
{
    qDeleteAll(this->poses);
}

bool Motion::open(const QString &filename)
{
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QTextStream stream(&f);
    return this->build(stream);
}

bool Motion::build(QTextStream &stream)
{
    QString prop_name = stream.readLine();
    QString prop_value = stream.readLine();
    if(!this->buildProperty(prop_name, prop_value)){
        return false;
    }
    this->markers = stream.readLine().split(",");
    bool dst = true;
    while(!stream.atEnd() && dst){
        QString line = stream.readLine();
        Pose* pose = new Pose();
        if(!this->buildPose(line, pose)){
            dst = false;
            delete pose;
        }
    }
    return dst;
}

bool Motion::buildProperty(const QString &name, const QString &value){
    QStringList names = name.split(",");
    QStringList vals = value.split(",");
    if(names.size() != vals.size()){
        return false;
    }
    for(int i = 0;i<names.size();++i){
        bool is_float = true;
        float float_value = vals[i].toFloat(*is_float);
        if(is_float){
            this->properties[names[i]] = float_value;
        }
    }
    return true;
}

bool Motion::buildPose(const QString &line, Pose *pose){
    bool dst = true;
    QStringList cells = line.split(",");
    int frame =cells[0].toInt();
    for(int i = 2;i<cells.size() && dst;i+=3){
        bool x_ok, y_ok, z_ok;
        float x = cells[i].toFloat(&x_ok);
        float y = cells[i].toFloat(&y_ok);
        float z = cells[i].toFloat(&z_ok);
        if(x_ok && y_ok && z_ok){
            pose->addJointData(markers[(i - 2) / 3], x, y, z);
        }
    }
    if(dst){
        this->poses[frame] = pose;
    }
    return dst;
}

const Pose& Motion::at(int frame) const{
    return *this->poses[frame];
}

Pose& Motion::at(int frame){
    return *this->poses[frame];
}
