#ifndef MOTIONGEOMETRYENGINE_H
#define MOTIONGEOMETRYENGINE_H
#include <QList>
#include <QBasicTimer>
#include "pose.h"
class Motion;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLTexture;
class BoxGeometryEngine;
class DrawConfigure;

/*!
 * \brief モーションを描画する諸々を保持するクラス
 *
 * モーションを描画するのに必要となる，
 *
 * \li BoxGeometryEngine
 * \li DrawConfigure
 * \li テクスチャ
 *
 * を内部的に保持しています．
 * また，それらを活用することにより，ポーズを描画することが可能です．
 *
 * ポーズの描画のために，このクラスは多少複雑な処理を行っていますので，内容について詳細に書いておきます．
 * まず，このクラスはDrawConfigureを参照します．
 * それにより，ポーズの描画にあたって必要となる，関節の接続情報を取得します．
 * 次に，関節の接続の数(ボーンの数)だけBoxGeometryEngineを作成します．
 * 最後に，各ボーンに対応するように長さや位置を設定してBoxを描画することで，スティックピクチャを描画します．
 */
class MotionGeometryEngine
{
public:
    /*!
     * \brief コンストラクタ
     */
    MotionGeometryEngine();

    /*!
     * \brief デストラクタ
     */
    virtual ~MotionGeometryEngine();

    /*!
     * \brief 必要な情報をもとに，Poseを描画します
     * \param program シェーダプログラム
     * \param vp_matrix view行列とprojection行列をかけたもの
     * \param pose 描画するPose
     */
    void drawMotionGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix, const Pose* pose);

private:
    QList<BoxGeometryEngine*> boxes;
    QBasicTimer timer;
    DrawConfigure* config;
    QOpenGLTexture* texture;

    void initTexture();
    void initBoxes();
    QMatrix4x4 calcModelMatrix(const QVector3D& first, const QVector3D& second);
};

#endif // MOTIONGEOMETRYENGINE_H
