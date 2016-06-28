#ifndef MOTIONBUILDER_H
#define MOTIONBUILDER_H

#include <QObject>

class QString;

class Motion;
class Pose;

/*!
 * \brief 各種ファイルからMotionクラスを構築する手法を提供します
 */
class MotionBuilder : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief ファイルからMotionクラスを構築します
     * \param filename ファイル名
     * \param parent Motionインスタンスの親
     * \return Motionインスタンス
     *
     * ファイルがどのような形式であるかは拡張子によって判断されます．
     * 拡張子によって自動的に判断され，内部的に関数を使い分けています．
     * もし対応していないファイル形式が渡された場合にはnullptrを返します．
     *
     * \sa Motion
     */
    static Motion* open(const QString& filename, QObject* parent=0);

private:
    explicit MotionBuilder(QObject *parent = 0);
    static Motion* buildFromCSV(const QString& filename, QObject* parent=0);
    static Motion* buildFromTRC(const QString& filename, QObject* parent=0);
    static Motion* buildFromTS(const QString& filename, QObject* parent=0);
    static Motion* buildFromXLSX(const QString& filenname, QObject* parent=0);

    static bool buildPoseFromCSV(const QStringList& cells, Motion* motion);
    static bool buildPoseFromTRC(const QStringList& cells, Motion* motion);
    static bool buildPoseFromTS(const QStringList& cells, Motion* motion);
};

#endif // MOTIONBUILDER_H
