#ifndef MOTIONVIEWERWIDGET_H
#define MOTIONVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>

class QTimer;
class MotionGeometryEngine;
class PlaneGeometryEngine;
class Motion;

class MotionViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MotionViewerWidget(QWidget* parent = 0);
    ~MotionViewerWidget();

    bool isPlaying()const;

public slots:
    void openMotionFile(const QString& filename);
    void play();
    void stop();
    void onMotionChanged();
    void setCurrentFrame(int frame);
    void updateMotionProperties();
    void updateCurrentFrame();

signals:
    void motionChanged();
    void currentFrameChanged(int frame);
    void playStateChanged(bool is_playing);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

private:
    const int FRAME_UPDATE_MSEC = 33;
    int timer_tick_count = 0;
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
