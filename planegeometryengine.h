#ifndef PLANEGEOMETRYENGINE_H
#define PLANEGEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class QOpenGLShaderProgram;
class QOpenGLTexture;

/*!
 * \brief 平面を描画するクラス
 */
class PlaneGeometryEngine : protected QOpenGLFunctions
{
public:
    /*!
     * \brief コンストラクタ
     * \param scale 平面の大きさを表すパラメータ
     *
     * ここで指定できるパラメータであるscaleは，平面の大きさを表します．
     * 実際には，scale / 500 の大きさに描画されます．
     */
    PlaneGeometryEngine(float scale = 1.0);

    /*!
     * \brief デストラクタ
     */
    ~PlaneGeometryEngine();

    /*!
     * \brief 描画します
     * \param program シェーダプログラム
     * \param vp_matrix view行列とprojection行列をかけたもの
     */
    void draw(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix);
private:
    void initTexture();
    void initGeometry();

    float scale;
    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexbuf;
    QOpenGLTexture *texture;
};

#endif // PLANEGEOMETRYENGINE_H
