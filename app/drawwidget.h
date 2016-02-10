#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

// this needs to be included first for GLEW to be working
#include <SparrowRenderer/sparrowrenderer.h>

#include <QGLWidget>
#include <QTimer>

#include "mycamera.h"
#include "scenemanager.h"

class ForwardModule;

class DrawWidget : public QGLWidget
{
    Q_OBJECT

    SparrowRenderer renderer;
    MyCamera camera;
    QTimer glRefreshTimer;
    SceneManager sceneManager;
    ForwardModule *forward;

    QPoint lastMousePos;
    bool grabbed;

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
};

#endif // DRAWWIDGET_H
