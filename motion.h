#ifndef MOTION_H
#define MOTION_H

#include <QObject>
#include <QMap>
#include <QTextStream>

#include "pose.h"

/*!
 * \brief モーションを表すクラス
 */
class Motion : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief コンストラクタ
     * \param parent 親
     */
    explicit Motion(QObject *parent = 0);

    /*!
     * \brief デストラクタ
     */
    ~Motion();

    /*!
     * \brief インデクサのオーバーロード
     * \param frame フレーム番号
     * \return フレームにおける姿勢
     *
     * 与えられたフーレム番号における姿勢を返します．
     * at関数と同じ意味を持ちますが，呼び出す場合にこちらの方が容易である場合も多いでしょう．
     *
     * \sa at(int frame) const
     */
    const Pose* operator[](int frame)const{
        return this->at(frame);
    }

    /*!
     * \brief インデクサのオーバーロード
     * \param frame フレーム番号
     * \return フレームにおける姿勢
     *
     * 与えられたフーレム番号における姿勢を返します．
     * at関数と同じ意味を持ちますが，呼び出す場合にこちらの方が容易である場合も多いでしょう．
     *
     * \sa at(int frame)
     */
    Pose* operator[](int frame){
        return this->at(frame);
    }


    /*!
     * \brief フレーム番号における姿勢を返します
     * \param frame フレーム番号
     * \return 姿勢
     */
    const Pose* at(int frame)const;

    /*!
     * \brief フレーム番号における姿勢を返します
     * \param frame フレーム番号
     * \return 姿勢
     */
    Pose* at(int frame);

    /*!
     * \brief あるフレーム番号の姿勢を有するかを返します
     * \param frame フレーム番号
     * \return 姿勢をデータとして持っているならtrue
     */
    bool contain(int frame)const;

    /*!
     * \brief フレーム番号における姿勢を設定します．
     * \param frame フレーム番号
     * \param pose 姿勢
     */
    void set(int frame, Pose* pose);

    /*!
     * \brief fpsを取得します
     * \return FPS
     */
    float fps()const;

    /*!
     * \brief fpsを設定します
     * \param fps 設定値
     */
    void setFps(float fps);

    /*!
     * \brief フレーム値の最大値を返します
     * \return 最大値
     */
    int maxFlame()const;

    /*!
     * \brief フレーム値の最大値を設定します
     * \param max_frame 設定する値
     */
    void setMaxFlame(int max_frame);

    /*!
     * \brief マーカ文字列を返します
     * \return マーカ文字列のリスト
     *
     * 注意する必要があるのは，ここで言われるマーカ文字列は特異な形式をしていることです．
     * 例えば，trc形式の場合，ファイルのマーカは次のように格納されています
     * > Frame,Time,Head,,,Hip,,...
     * この場合，マーカ文字列のリストは以下のように構成されます
     * > ["Frame", "Time", "Head", "", "", "Hip", "", "",...]
     * うっかりすると大惨事なので気をつけましょう
     *
     * \todo
     * わかりにくい形式であり，明らかに内部仕様が漏れだしているので修正が必要．
     * 修正が他のクラスに伝播するため，注意が必要．
     */
    const QStringList& markers()const;

    /*!
     * \brief マーカ文字列を設定します．
     * \param markers 設定値
     */
    void setMarkers(const QStringList& markers);

signals:

public slots:

private:
    QMap<int, Pose*> poses;
    QStringList markers_;
    float fps_;
    int maxFrame_;
};

#endif // MOTION_H
