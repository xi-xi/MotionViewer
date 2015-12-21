#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class MotionViewerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile();
    void openMotionFile(const QString& filename);
    void configure();

signals:
    void motionFileChanged(const QString& filename);

private:
    QMenu *filemenu;
    QAction *openaction;
    QAction *configureaction;
    QAction *closeaction;
    MotionViewerWidget *viewerwidget;

    void initUI();
    void initLogic();
};

#endif // MAINWINDOW_H
