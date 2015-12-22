#ifndef MOTIONVIEWERWIDGET_H
#define MOTIONVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class QTimer;
class MotionGeometryEngine;
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

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

private:
    const int FRAME_UPDATE_MSEC = 10;
    int timer_tick_count = 0;
    int fps = 120;

    bool playing;

    int current_frame;
    int max_frame;
    Motion* motion;
    bool motion_loaded;

    MotionGeometryEngine *geometries;

    QTimer *timer;

    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;
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
