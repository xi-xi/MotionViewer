#ifndef DRAWCONFIGURE_H
#define DRAWCONFIGURE_H
#include <QObject>

#include <QList>
#include <QPair>
#include <QString>
#include <QVector3D>

/*!
 * \brief 描画に関する設定
 *
 * どの関節とどの関節を繋いで描画するのかをといった設定を保持します．
 * 一応Jsonファイルもサポートしています．
 */
class DrawConfigure : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief json形式のファイルから設定を読みだします
     * \param filename ファイルパス
     * \param parent オブジェクトの親となるもの
     * \return 描画設定
     */
    static DrawConfigure* fromJsonFile(const QString& filename, QObject* parent = 0);

    /*!
     * \brief json形式文字列からファイル設定を作ります
     * \param text 文字列
     * \param parent 設定オブジェクトの親となるもの
     * \return 描画設定
     */
    static DrawConfigure* fromJsonString(const QString& text, QObject* parent = 0);

    /*!
     * \brief デフォルトの描画設定を返します
     * \param parent 親になるもの
     * \return 描画設定
     */
    static DrawConfigure* defaultConfigure(QObject* parent = 0);

    /*!
     * \brief コンストラクタ
     * \param parent 親
     */
    explicit DrawConfigure(QObject *parent = 0);

    /*!
     * \brief 接続すると設定されている関節のペアを返す
     * \return 接続する関節ペア
     *
     * 関節は関節名で識別されます．
     * そのため，戻り値の型はQPair<QString, QString>です．
     */
    const QList<QPair<QString, QString>>& getConnectJoints();

    /*!
     * \brief 接続する関節を増やします
     * \param a 関節
     * \param b 関節
     *
     * ２つの関節名を引数として，それらを接続します．
     */
    void addConnectJoint(QString a, QString b);

    /*!
     * \brief 描画に用いる箱のサイズ
     */
    QVector3D box_size;
signals:

public slots:
private:
    QList<QPair<QString, QString>> connect_joints;

};

#endif // DRAWCONFIGURE_H
