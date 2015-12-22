#include "drawconfigure.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DrawConfigure* DrawConfigure::fromJsonFile(const QString &filename,QObject* parent){
    QFile f(filename);
    if(f.open(QFile::ReadOnly)){
        QTextStream stream(&f);
        return DrawConfigure::fromJsonString(stream.readAll(), parent);
    }
    return DrawConfigure::defaultConfigure(parent);
}

DrawConfigure* DrawConfigure::fromJsonString(const QString &text,QObject* parent){
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject obj = doc.object();
    if(!obj.empty()){
        auto conf = new DrawConfigure(parent);
        if(obj.contains("joints")){
            QJsonArray joints = obj["joints"].toArray();
            for(int i = 0;i<joints.size();++i){
                QJsonArray vals = joints[i].toArray();
                conf->addConnectJoint(vals[0].toString(), vals[1].toString());
            }
        }
        if(obj.contains("box_size")){
            QJsonArray vector = obj["box_size"].toArray();
            conf->box_size = QVector3D(vector[0].toDouble(), vector[1].toDouble(), vector[2].toDouble());
        }
        return conf;
    }
    return new DrawConfigure(parent);
}

DrawConfigure* DrawConfigure::defaultConfigure(QObject *parent){
    auto conf = new DrawConfigure(parent);
    conf->addConnectJoint("Top.Head", "Back.Head");
    conf->addConnectJoint("Top.Head", "Front.Head");
    conf->addConnectJoint("Top.Head", "L.Head_Offset");
    conf->addConnectJoint("Front.Head", "Back.Head");
    conf->addConnectJoint("Top.Head", "Neck");
    conf->addConnectJoint("Back.Head", "Neck");
    conf->addConnectJoint("Front.Head", "Neck");
    conf->addConnectJoint("Neck", "R.Shoulder");
    conf->addConnectJoint("Neck", "L.Shoulder");
    conf->addConnectJoint("R.Shoulder", "L.BackOffset");
    conf->addConnectJoint("L.Shoulder", "Back.Head");
    conf->addConnectJoint("R.Shoulder", "R.Bicep");
    conf->addConnectJoint("R.Bicep", "R.Elbow");
    conf->addConnectJoint("R.Shoulder", "R.Elbow");
    conf->addConnectJoint("R.Elbow", "R.ForeArm");
    conf->addConnectJoint("R.ForeArm", "R.Radius");
    conf->addConnectJoint("R.ForeArm", "R.Ulna");
    conf->addConnectJoint("R.ForeArm", "R.Thumb");
    conf->addConnectJoint("R.ForeArm", "R.Pinky");
    conf->addConnectJoint("L.Shoulder", "L.Bicep");
    conf->addConnectJoint("L.Bicep", "L.Elbow");
    conf->addConnectJoint("L.Shoulder", "L.Elbow");
    conf->addConnectJoint("L.Elbow", "L.ForeArm");
    conf->addConnectJoint("L.ForeArm", "L.Radius");
    conf->addConnectJoint("L.ForeArm", "L.Ulna");
    conf->addConnectJoint("L.ForeArm", "L.Thumb");
    conf->addConnectJoint("L.ForeArm", "L.Pinky");
    conf->addConnectJoint("Neck", "V.Sacral");
    conf->addConnectJoint("R.PSIS", "V.Sacral");
    conf->addConnectJoint("R.ASIS", "R.PSIS");
    conf->addConnectJoint("L.PSIS", "V.Sacral");
    conf->addConnectJoint("L.ASIS", "L.PSIS");
    conf->addConnectJoint("R.ASIS", "R.Thigh");
    conf->addConnectJoint("R.Thigh", "R.Knee");
    conf->addConnectJoint("R.Knee", "R.Shank");
    conf->addConnectJoint("R.Shank", "R.Ankle");
    conf->addConnectJoint("R.Ankle", "R.Heel");
    conf->addConnectJoint("R.Heel","R.Foot");
    conf->addConnectJoint("R.Heel","R.Toe");
    conf->addConnectJoint("R.Toe","R.Foot");
    conf->addConnectJoint("L.ASIS", "L.Thigh");
    conf->addConnectJoint("L.Thigh", "L.Knee");
    conf->addConnectJoint("L.Knee", "L.Shank");
    conf->addConnectJoint("L.Shank", "L.Ankle");
    conf->addConnectJoint("L.Ankle", "L.Heel");
    conf->addConnectJoint("L.Heel","L.Foot");
    conf->addConnectJoint("L.Heel","L.Toe");
    conf->addConnectJoint("L.Toe","L.Foot");
    conf->box_size = QVector3D(10, 10, 10);
    return conf;
}

DrawConfigure::DrawConfigure(QObject *parent) : QObject(parent)
{

}

const QList<QPair<QString, QString>>& DrawConfigure::getConnectJoints(){
    return this->connect_joints;
}

void DrawConfigure::addConnectJoint(QString a, QString b){
    this->connect_joints.append(QPair<QString, QString>(a, b));
}
