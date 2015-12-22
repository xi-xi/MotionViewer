#ifndef DRAWCONFIGURE_H
#define DRAWCONFIGURE_H
#include <QObject>

#include <QList>
#include <QPair>
#include <QString>
#include <QVector3D>

class DrawConfigure : public QObject
{
    Q_OBJECT
public:
    static DrawConfigure* fromJsonFile(const QString& filename, QObject* parent = 0);
    static DrawConfigure* fromJsonString(const QString& text, QObject* parent = 0);
    static DrawConfigure* defaultConfigure(QObject* parent = 0);
    explicit DrawConfigure(QObject *parent = 0);
    const QList<QPair<QString, QString>>& getConnectJoints();
    void addConnectJoint(QString a, QString b);
    QVector3D box_size;
signals:

public slots:
private:
    QList<QPair<QString, QString>> connect_joints;

};

#endif // DRAWCONFIGURE_H
