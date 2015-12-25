#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QToolBar;
class QLabel;
class MotionViewerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);

public slots:
    void openFile();
    void openMotionFile(const QString& filename);
    void configure();
    void playstopButtonClicked();
    void backButtonClicked();
    void nextButtonClicked();
    void setPlayStopButtonState(bool isplaying);
    void onMotionFileChanged(const QString& filename);

signals:
    void motionFileChanged(const QString& filename);

private:
    QLabel *filename;
    QMenu *filemenu;
    QAction *openaction;
    QAction *configureaction;
    QAction *closeaction;
    MotionViewerWidget *viewerwidget;
    QToolBar *operationbar;
    QAction *back;
    QAction *play_stop;
    QAction *next;

    void initUI();
    void initLogic();
};

#endif // MAINWINDOW_H
