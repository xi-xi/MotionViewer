#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openMotionFile(const QString& filename);

signals:
    void motionFileChanged(const QString& filename);

private:
};

#endif // MAINWINDOW_H
