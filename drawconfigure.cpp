#include "drawconfigure.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

DrawConfigure* DrawConfigure::fromJsonFile(const QString &filename,QObject* parent){
    QFile f(filename);
    if(f.open(QFile::ReadOnly)){
        QTextStream stream(&f);
        return DrawConfigure::fromJsonString(stream.readAll(), parent);
    }
    return DrawConfigure::defaultConfigure(parent);
}

DrawConfigure* DrawConfigure::fromJsonString(const QString &text,QObject* parent){
    QJsonDocument doc = QJsonDocument::fromJson(text);
    QJsonObject obj = doc.object();
    return new DrawConfigure(parent);
}

DrawConfigure* DrawConfigure::defaultConfigure(QObject *parent){
    return new DrawConfigure(parent);
}

DrawConfigure::DrawConfigure(QObject *parent) : QObject(parent)
{

}
