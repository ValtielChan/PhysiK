#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

// this needs to be included first for GLEW to be working
#include <SparrowRenderer/sparrowrenderer.h>

#include <QOpenGLWidget>
#include <QTimer>

#include "mycamera.h"
#include "scenemanager.h"

class ForwardModule;
class PickFramebuffer;

class DrawWidget : public QOpenGLWidget
{
    Q_OBJECT

    SparrowRenderer renderer;
    MyCamera camera;
    QTimer glRefreshTimer;
    SceneManager sceneManager;
    ForwardModule *forward;
    PickFramebuffer *fbo;

    QPoint lastMousePos;
    bool grabbedLeft;
    bool grabbedRight;
    bool paused;

    void initPipeline();
    glm::vec3 getRandomPos();

protected:
    // Output
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    // Input
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    DrawWidget(QWidget *parent = 0);

public slots:
    void addMesh();
    void addParticles();
    void resetScene();

private slots:
    void update();
};

#endif // DRAWWIDGET_H
