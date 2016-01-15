#include "motionbuilder.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "xlsxdocument.h"
#include "motion.h"

MotionBuilder::MotionBuilder(QObject *parent) : QObject(parent)
{

}

Motion* MotionBuilder::open(const QString &filename, QObject *parent)
{
    QFileInfo info(filename);
    auto ext = info.suffix().toLower();
    if(ext == "trc"){
        return MotionBuilder::buildFromTRC(filename, parent);
    }
    else if(ext == "ts"){
        return MotionBuilder::buildFromTS(filename, parent);
    }
    else if(ext == "csv"){
        return MotionBuilder::buildFromCSV(filename, parent);
    }
    else if(ext == "xlsx"){
        return MotionBuilder::buildFromXLSX(filename, parent);
    }
    return nullptr;
}

Motion* MotionBuilder::buildFromTRC(const QString &filename, QObject *parent){
    const QChar sep = '\t';
    const QChar rm = '\r';
    const QStringList DEFAULT_TRC_MARKERS = QString(
        "Frame#	Time	Top.head			Back.Head			Front.Head			L.Head_Offset			R.Shoulder			L.Shoulder			Neck			L.BackOffset			R.Bicep			R.Elbow			R.ForeArm			R.Radius			R.Ulna			R.Thumb			R.Pinky			L.Bicep			L.Elbow			L.Forearm			L.Radius			L.Ulna			L.Thumb			L.Pinky			R.ASIS			L.ASIS			R.PSIS			L.PSIS			V.Sacral			R.Thigh			R.Knee			R.Shank			R.Ankle			R.Heel			R.Toe			R.Foot			L.Thigh			L.Knee			L.Shank			L.Ankle			L.Toe			L.Heel			L.Foot			"
    ).split(sep);
    Motion* motion = new Motion(parent);
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return nullptr;
    }
    QTextStream stream(&f);
    stream.readLine();  // ignore PathFileType
    QStringList prop_names = stream.readLine().remove(rm).split(sep);
    QStringList prop_values = stream.readLine().remove(rm).split(sep);
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
        else if(prop_names[i] == "NumFrames"){
            bool is_int = false;
            int val = prop_values[i].toInt(&is_int);
            if(is_int){
                motion->setMaxFlame(val);
            }
        }
    }
    QStringList markers = stream.readLine().remove(rm).split(sep);
    if(markers.size() <= 3){
        markers = DEFAULT_TRC_MARKERS;
    }
    motion->setMarkers(markers);
    stream.readLine();
    stream.readLine();
    bool dst = true;
    while(!stream.atEnd()){
        QStringList cells = stream.readLine().remove(rm).split(sep);
        if(!MotionBuilder::buildPoseFromTRC(cells, motion))
        {
            dst = false;
        }
    }
    if(!dst){
        return nullptr;
    }
    return motion;
}

Motion* MotionBuilder::buildFromTS(const QString &filename, QObject *parent){
    const QChar sep = '\t';
    const QChar rm = '\r';
    Motion* motion = new Motion(parent);
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return nullptr;
    }
    QTextStream stream(&f);
    stream.readLine();  // ignore PathFileType
    QStringList prop_names = stream.readLine().remove(rm).split(sep);
    QStringList prop_values = stream.readLine().remove(rm).split(sep);
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
        else if(prop_names[i] == "NumFrames"){
            bool is_int = false;
            int val = prop_values[i].toInt(&is_int);
            if(is_int){
                motion->setMaxFlame(val);
            }
        }
    }
    QStringList markers = stream.readLine().remove(rm).split(sep);
    motion->setMarkers(markers);
    stream.readLine();
    stream.readLine();
    bool dst = true;
    while(!stream.atEnd()){
        QStringList cells = stream.readLine().remove(rm).split(sep);
        if(!MotionBuilder::buildPoseFromTS(cells, motion))
        {
            dst = false;
        }
    }
    if(!dst){
        return nullptr;
    }
    return motion;
}

Motion* MotionBuilder::buildFromCSV(const QString &filename, QObject *parent){
    const QChar sep = ',';
    Motion* motion = new Motion(parent);
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
        QStringList cells = stream.readLine().split(sep);
        if(!MotionBuilder::buildPoseFromCSV(cells, motion))
        {
            dst = false;
        }
    }
    if(!dst){
        return nullptr;
    }
    return motion;
}

bool MotionBuilder::buildPoseFromCSV(const QStringList &cells, Motion *motion)
{
    bool dst = true;
    Pose* pose = new Pose(motion);
    int frame = cells[0].toInt();
    for(int i = 2;i<motion->markers().size()-2 && i < cells.size() -2;i+=3){
        bool x_ok, y_ok, z_ok;
        float x = cells[i].toFloat(&x_ok);
        float y = cells[i+1].toFloat(&y_ok);
        float z = cells[i+2].toFloat(&z_ok);
        if(x_ok && y_ok && z_ok){
            pose->addJointData(motion->markers()[i], x, y, z);
        }
    }
    if(dst){
        motion->set(frame, pose);
    }
    return dst;
}

bool MotionBuilder::buildPoseFromTRC(const QStringList &cells, Motion *motion)
{
    Pose* pose = new Pose(motion);
    int frame = cells[0].toInt();
    for(int i = 2;i < motion->markers().size()-2 && i < cells.size() - 2;i+=3){
        bool x_ok, y_ok, z_ok;
        float x = cells[i].toFloat(&x_ok);
        float y = cells[i+1].toFloat(&y_ok);
        float z = cells[i+2].toFloat(&z_ok);
        if(x_ok && y_ok && z_ok){
            pose->addJointData(motion->markers()[i], x, y, z);
        }
    }
    motion->set(frame, pose);
    return true;
}

bool MotionBuilder::buildPoseFromTS(const QStringList &cells, Motion *motion)
{
    Pose* pose = new Pose(motion);
    int frame = cells[0].toInt();
    for(int i = 2;i < motion->markers().size()-10 && i < cells.size() - 10;i+=11){
        bool x_ok, y_ok, z_ok;
        float x = cells[i].toFloat(&x_ok);
        float y = cells[i+1].toFloat(&y_ok);
        float z = cells[i+2].toFloat(&z_ok);
        if(x_ok && y_ok && z_ok){
            pose->addJointData(motion->markers()[i], x, y, z);
        }
    }
    motion->set(frame, pose);
    return true;
}

Motion* MotionBuilder::buildFromXLSX(const QString &filenname, QObject *parent)
{
    const int COLUMN_RANGE = 71;
    const QStringList MARKER_LABELS = QString(
        ",Frame#,Time,Hip,,,L5,,,L3,,,T12,,,T8,,,Neck,,,Head,,,R.Shoulder,,,R.UpperArm,,,R.ForeArm,,,R.Hand,,,L.Shoulder,,,L.UpperArm,,,L.ForeArm,,,L.Hand,,,R.Thigh,,,R.Shin,,,R.Ankle,,,R.Foot,,,L.Thigh,,,L.Shin,,,L.Ankle,,,L.Foot"
    ).split(",");
    Motion* motion = new Motion(parent);
    motion->setFps(120);
    motion->setMarkers(MARKER_LABELS);
    QXlsx::Document xlsx(filenname);
    QXlsx::Worksheet *sheet = dynamic_cast<QXlsx::Worksheet*>(xlsx.sheet(xlsx.sheetNames()[0]));
    QXlsx::CellRange range = sheet->dimension();
    int last_row = range.lastRow();
    int max_frame = 0;
    for(int y = 2;y<=last_row;++y){
        int frame = sheet->cellAt(y, 1)->value().value<float>();
        if(frame <= 0){
            continue;
        }
        if(frame > max_frame){
            max_frame = frame;
        }
        Pose* pose = new Pose(motion);
        for(int x = 3;x<=COLUMN_RANGE - 2;x += 3){
            QXlsx::Cell* cell_x = sheet->cellAt(y, x);
            QXlsx::Cell* cell_y = sheet->cellAt(y, x + 1);
            QXlsx::Cell* cell_z = sheet->cellAt(y, x + 2);
            if(cell_x && cell_y && cell_z){
                pose->addJointData(
                    motion->markers()[x],
                    cell_y->value().value<float>() * 1000,
                    cell_z->value().value<float>() * 1000,
                    cell_x->value().value<float>() * 1000
                );
            }
        }
        motion->set(frame, pose);
    }
    motion->setMaxFlame(max_frame);
    return motion;
}
