#ifndef POSE_H
#define POSE_H

#include <QObject>
#include <QMap>
#include <QVector3D>

/*!
 * \brief ある瞬間の姿勢を表すクラス
 */
class Pose : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief コンストラクタ
     * \param parent 親
     */
    explicit Pose(QObject *parent = 0);

    /*!
     * \brief 関節データを追加します
     * \param name 関節名
     * \param position 関節位置
     */
    void addJointData(const QString& name, const QVector3D& position);

    /*!
     * \brief 関節データを追加します
     * \param name 関節名
     * \param x 関節位置(x)
     * \param y 関節位置(y)
     * \param z 関節位置(z)
     */
    void addJointData(const QString &name, float x, float y, float z);

    /*!
     * \brief インデクサのオーバーロード
     * \param name 関節名
     * \return 関節位置
     *
     * 関節名から関節位置を返してくれます
     *
     * \sa at()
     */
    const QVector3D operator[](const QString& name)const{
        return this->at(name);
    }

    /*!
     * \brief 関節名から関節位置を返します
     * \param name 関節名
     * \return 関節位置
     */
    const QVector3D at(const QString& name)const;

    /*!
     * \brief この姿勢が関節名を含むかを返します
     * \param name 関節名
     * \return 含むならtrue
     */
    bool contain(const QString& name)const;

    /*!
     * \brief 関節名のリストを返します
     * \return 関節名の一覧
     */
    QList<QString> getJointsName()const;

signals:

public slots:

private:
    QMap<QString, QVector3D> joint_positions;
};

#endif // POSE_H
