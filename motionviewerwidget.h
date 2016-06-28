#ifndef MOTIONVIEWERWIDGET_H
#define MOTIONVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <chrono>

class QTimer;
class MotionGeometryEngine;
class PlaneGeometryEngine;
class Motion;

/*!
 * \brief モーションを描画するウィジェット
 */
class MotionViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    /*!
     * \brief コンストラクタ
     * \param parent 親
     */
    explicit MotionViewerWidget(QWidget* parent = 0);

    /*!
     * \brief デストラクタ
     */
    ~MotionViewerWidget();

    /*!
     * \brief モーションが再生中であるかを返します
     * \return 再生中ならtrue
     */
    bool isPlaying()const;

    /*!
     * \brief 再生対象となるMotionを取得
     * \return 再生対象のMotion
     */
    const Motion* getMotion()const;

    /*!
     * \brief モーションが読み込まれているかを返します
     * \return 読み込まれていればtrue
     *
     * モーションをロードした場合，
     * 何らかの要因(対応していないファイル形式，ファイルが無いなど)でロードに失敗する可能性があります．
     * その場合に，この関数はfalseを返します．
     */
    bool motionLoaded()const;

public slots:
    /*!
     * \brief モーションファイルを開きます
     * \param filename ファイル名
     */
    void openMotionFile(const QString& filename);

    /*!
     * \brief モーションの再生を開始します
     */
    void play();

    /*!
     * \brief モーションの再生を停止します
     */
    void stop();

    /*!
     * \brief モーションファイルが変更された時に呼ばれます
     */
    void onMotionChanged();

    /*!
     * \brief 現在のフレーム番号を設定します
     * \param frame 設定値
     */
    void setCurrentFrame(int frame);

    /*!
     * \brief モーションのプロパティを再設定します
     *
     * モーションのプロパティとは，FPSと最大フレーム数です．
     */
    void updateMotionProperties();

    /*!
     * \brief 現在の描画フレーム数を更新します．
     *
     * この処理には，再生開始からの経過時間を測定し，
     * その経過時間とFPSから現在のあるべきフレーム番号を計算する処理が含まれます．
     */
    void updateCurrentFrame();

signals:
    /*!
     * \brief モーションが変更された時にemit
     */
    void motionChanged();

    /*!
     * \brief 現在のフレーム番号が変更された時にemit
     * \param frame 変更後のフレーム番号
     */
    void currentFrameChanged(int frame);

    /*!
     * \brief 再生状態が変更された時にemit
     * \param is_playing 再生中ならtrue
     */
    void playStateChanged(bool is_playing);

protected:
    /*!
     * \brief OpenGLの初期化
     */
    void initializeGL();

    /*!
     * \brief OpenGLのりサイズイベント
     * \param w 横幅
     * \param h 高さ
     */
    void resizeGL(int w, int h);

    /*!
     * \brief OpenGLコンテンツを描画
     */
    void paintGL();

    /*!
     * \brief GLに必要なシェーダを初期化します
     */
    void initShaders();

    /*!
     * \brief マウスボタンが押された時のイベント
     * \param event イベントオブジェクト
     */
    virtual void mousePressEvent(QMouseEvent* event);

    /*!
     * \brief マウスが移動した時のイベント
     * \param event イベントオブジェクト
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

    /*!
     * \brief マウスホイール回転時のイベント
     * \param event イベントオブジェクト
     */
    virtual void wheelEvent(QWheelEvent* event);

private:
    typedef std::chrono::system_clock Time;
    Time::time_point start_time;
    const int FRAME_UPDATE_MSEC = 0;
    int start_frame = 1;
    int fps = 120;

    bool playing;

    int current_frame;
    int max_frame;
    Motion* motion;
    bool motion_loaded;

    MotionGeometryEngine *geometries;
    PlaneGeometryEngine *plane;

    QTimer *timer;

    QOpenGLShaderProgram program;
    QMatrix4x4 projection;

    QPoint mouseclicked_position;

    const qreal zNear = 3.0;
    const qreal zFar = 10000.0;
    qreal aspect;
    qreal fov = 45.0;
    const qreal FOV_UPPER_LIMIT = 80;
    const qreal FOV_DOWN_LIMIT = -80;
    void updatePerspective();

    QVector3D camera_translate = QVector3D(0, 750, 2500);
    qreal camera_angle = .0;
};

#endif // MOTIONVIEWERWIDGET_H
