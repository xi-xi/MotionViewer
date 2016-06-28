#ifndef BOXGEOMETRYENGINE_H
#define BOXGEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

/*!
 * \brief 箱を描画するクラス
 *
 * サイズや変換行列を指定し，箱を描画する能力を持ちます．
 */
class BoxGeometryEngine : protected QOpenGLFunctions
{
public:
    /*!
     * \brief コンストラクタ
     *
     * 特に何もしない．
     * サイズは2.0, 2.0, 2.0の大きさになります
     */
    BoxGeometryEngine();

    /*!
     * \brief BoxGeometryEngineのサイズを指定できるコンストラクタ
     * \param w 横幅
     * \param h 縦幅
     * \param d 奥行き
     */
    BoxGeometryEngine(float w, float h, float d);

    /*!
     * \brief デストラクタ
     *
     * 内部的に保持している要素を削除します
     */
    virtual ~BoxGeometryEngine();

    /*!
     * \brief 描画します
     * \param program シェーダプログラム．これを用いて描画します．
     * \param vp_matrix view行列とprojection行列を掛けたもの．これにmodel行列をかけて実際に描画します．
     */
    void drawBoxGeometry(QOpenGLShaderProgram* program, const QMatrix4x4& vp_matrix);

    /*!
     * \brief 横幅を設定します
     * \param val 設定する値
     * \sa setWHD()
     * \sa getWidth()
     */
    void setWidth(float val);

    /*!
     * \brief 縦幅を設定します
     * \param val 設定する値
     * \sa setWHD()
     * \sa getHeight()
     */
    void setHeight(float val);

    /*!
     * \brief 奥行きを設定します
     * \param val 設定する値
     * \sa setWHD()
     * \sa getDepth()
     */
    void setDepth(float val);

    /*!
     * \brief 縦横奥行きを同時に指定します
     * \param w 幅
     * \param h 高さ
     * \param d 奥行き
     * \sa setWidth()
     * \sa setHeight()
     * \sa setDepth()
     */
    void setWHD(float w, float h, float d);

    /*!
     * \brief モデル行列を設定します
     * \param matrix 設定する行列
     *
     * モデル行列として用いられる行列を設定します．
     * ここで設定した行列はdrawBoxGeometry関数で参照されます．
     */
    void setModelMatrix(const QMatrix4x4& matrix);

    /*!
     * \brief 幅を取得します
     * \return 幅
     *
     * \sa setWidth()
     */
    float getWidth()const;

    /*!
     * \brief 高さを指定します
     * \return 高さ
     *
     * \sa setHeight()
     */
    float getHeight()const;

    /*!
     * \brief 奥行きを取得します
     * \return 奥行き
     *
     * \sa setDepth()
     */
    float getDepth()const;

    /*!
     * \brief モデル行列を取得します
     * \return モデル行列
     *
     * \sa setModelMatrix()
     */
    const QMatrix4x4& getModelMatrix()const;

private:
    void initBoxGeometry();
    void updateModelMatrix();

    float width;
    float height;
    float depth;

    QOpenGLBuffer arraybuf;
    QOpenGLBuffer indexbuf;
    QMatrix4x4 model_matrix;
};

#endif // BOXGEOMETRYENGINE_H
