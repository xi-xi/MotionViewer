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

class MotionViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MotionViewerWidget(QWidget* parent = 0);
    ~MotionViewerWidget();

    bool isPlaying()const;
    const Motion* getMotion()const;
    bool motionLoaded()const;

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
    QMatrix4x4 camera_matrix;

    QPoint mouse_left_clicked_position;
    QPoint mouse_right_clicked_position;

    const qreal zNear = 3.0;
    const qreal zFar = 10000.0;
    qreal aspect;
    const qreal fov = 45.0;
    void updatePerspective();
    void updateCameraMatrix();

    const qreal minimum_camera_radius = 1.0;
    const qreal maximum_camera_radius = 5000.0;
    const qreal default_camera_radius = 2500.0;
    const QVector3D default_camera_center = QVector3D(0, 800, 0);
    const QVector3D default_camera_up = QVector3D(0, 1, 0);
    const QVector3D default_camera_right = QVector3D(0, 0, -1);
    qreal camera_radius = default_camera_radius;
    QVector3D camera_center = default_camera_center;
    QVector3D camera_up = default_camera_up;
    QVector3D camera_right = default_camera_right;
    qreal camera_h_angle = .0;
    qreal camera_v_angle = .0;
    void addCameraHorizontalAngle(qreal angle);
    void addCameraVerticalAngle(qreal angle);
};

#endif // MOTIONVIEWERWIDGET_H
