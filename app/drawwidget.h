#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

// this needs to be included first for GLEW to be working
#include <SparrowRenderer/sparrowrenderer.h>

#include <QOpenGLWidget>
#include <QTimer>

#include "mycamera.h"
#include "scenemanager.h"

class ForwardModule;

class DrawWidget : public QOpenGLWidget
{
    Q_OBJECT

    SparrowRenderer renderer;
    MyCamera camera;
    QTimer glRefreshTimer;
    SceneManager sceneManager;
    ForwardModule *forward;

protected:
    // Output
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    void initPipeline();

    // Input
    QPoint lastMousePos;
    bool grabbed;
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public:
    DrawWidget(QWidget *parent = 0);
};

#endif // DRAWWIDGET_H
