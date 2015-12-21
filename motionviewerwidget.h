#ifndef MOTIONVIEWERWIDGET_H
#define MOTIONVIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

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

signals:
    void motionChanged();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

private:
    bool playing;

    int current_frame;
    Motion* motion;
    bool motion_loaded;

    MotionGeometryEngine *geometries;

    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;
    QMatrix4x4 projection;
};

#endif // MOTIONVIEWERWIDGET_H
