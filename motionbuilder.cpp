#include "motionbuilder.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "motion.h"

MotionBuilder::MotionBuilder(QObject *parent) : QObject(parent)
{

}

Motion* MotionBuilder::open(const QString &filename)
{
    QFileInfo info(filename);
    auto ext = info.suffix().toLower();
    if(ext == "trc"){
        return this->buildFromTRC(filename);
    }
    else if(ext == "ts"){
        return this->buildFromTS(filename);
    }
    else if(ext == "csv"){
        return this->buildFromCSV(filename);
    }
    return nullptr;
}

Motion* MotionBuilder::buildFromTRC(const QString &filename){
    return nullptr;
}

Motion* MotionBuilder::buildFromTS(const QString &filename){
    return nullptr;
}

Motion* MotionBuilder::buildFromCSV(const QString &filename){
    const QChar sep = ',';
    Motion* motion = new Motion(this->parent());
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return nullptr;
    }
    QTextStream stream(&f);
    QStringList prop_names = stream.readLine().split(sep);
    QStringList prop_values = stream.readLine().split(sep);
    if(prop_names.size() != prop_values.size()){
        return nullptr;
    }
    for(int i = 0;i<prop_names.size();++i){
        if(prop_names[i] == "DataRate"){
            bool is_float = false;
            float float_value = prop_values[i].toFloat(&is_float);
            if(is_float){
                motion->setFps(float_value);
            }
        }
        if(prop_names[i] == "NumFrames"){
            bool is_int = false;
            int val = prop_values[i].toInt(&is_int);
            if(is_int){
                motion->setMaxFlame(val);
            }
        }
    }
    motion->setMarkers(stream.readLine().split(sep));
    bool dst = true;
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(!MotionBuilder::buildPoseFromCSV(line, motion))
        {
            dst = false;
            delete pose;
        }
    }
    if(!dst){
        return nullptr;
    }
    return motion;
}

bool MotionBuilder::buildPoseFromCSV(const QString &line, Motion *motion)
{
    bool dst = true;
    QStringList cells = line.split(",");
    int frame =cells[0].toInt();
    for(int i = 2;i<cells.size();i+=3){
        bool x_ok, y_ok, z_ok;
        float x = cells[i].toFloat(&x_ok);
        float y = cells[i+1].toFloat(&y_ok);
        float z = cells[i+2].toFloat(&z_ok);
        if(x_ok && y_ok && z_ok){
            motion->addJointData(markers[i], x, y, z);
        }
    }
    if(dst){
        this->poses[frame] = motion;
    }
    return dst;
}
