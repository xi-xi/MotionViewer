#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QToolBar;
class QLabel;
class QSlider;
class QSpinBox;
class MotionViewerWidget;

/*!
 * \brief メインウインドウ
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief コンストラクタ
     * \param parent 親
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
     * \brief デストラクタ
     */
    ~MainWindow();

    /*!
     * \brief 何かがドラッグで持ち込まれた時に呼び出される
     * \param event イベントオブジェクト
     */
    virtual void dragEnterEvent(QDragEnterEvent* event);

    /*!
     * \brief 何かがドロップされた時に呼び出される
     * \param event イベントオブジェクト
     */
    virtual void dropEvent(QDropEvent* event);

    /*!
     * \brief キーが押された時に呼び出される
     * \param event イベントオブジェクト
     */
    virtual void keyPressEvent(QKeyEvent* event);

public slots:
    /*!
     * \brief ファイルを開くダイアログ表示などを行う
     */
    void openFile();

    /*!
     * \brief モーションファイルを開きます
     * \param filename ファイルパス
     */
    void openMotionFile(const QString& filename);

    /*!
     * \brief メニュー中の設定ボタンが押された時に呼び出される
     */
    void configure();

    /*!
     * \brief 再生/停止ボタンが押された時に呼び出される
     */
    void playstopButtonClicked();

    /*!
     * \brief 前へボタンが押された時に呼び出される
     */
    void backButtonClicked();

    /*!
     * \brief 次へボタンが押された時に呼び出される
     */
    void nextButtonClicked();

    /*!
     * \brief 再生中であるかのフラグを変更します
     * \param isplaying 再生中ならtrue
     */
    void setPlayStopButtonState(bool isplaying);

    /*!
     * \brief モーションファイルが変更されるときに呼び出される
     * \param filename 変更されるファイル名
     *
     * この関数はモーションファイルが変更されようとしている場合に呼び出されます．
     * 呼び出された段階では，モーションファイルはロードされておらず，ファイル名のみがわかる状態です．
     *
     * \sa onMotionFileChanged()
     */
    void onMotionFileChanging(const QString& filename);

    /*!
     * \brief モーションファイルが変更されたときに呼び出される
     *
     * この関数が呼び出された段階で，モーションファイルはロードされ，内容にアクセスすることが可能です．
     * ファイル内容に応じたもろもろの処理はこの関数内で行います．
     *
     * \sa onMotionFileChanging()
     */
    void onMotionFileChanged();

signals:
    /*!
     * \brief モーションファイルが変更されようとしている場合にemitされるsignalです
     * \param filename 読み込まれる予定のファイル名
     */
    void motionFileChanging(const QString& filename);

private:
    QMenu *filemenu;
    QAction *openaction;
    QAction *configureaction;
    QAction *closeaction;
    MotionViewerWidget *viewerwidget;
    QToolBar *operationbar;
    QAction *back;
    QAction *play_stop;
    QAction *next;
    QSlider *slider;
    QSpinBox *box;
    QLabel *maxframe_label;

    void initUI();
    void initLogic();
};

#endif // MAINWINDOW_H
