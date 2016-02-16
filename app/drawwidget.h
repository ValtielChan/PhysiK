#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

// this needs to be included first for GLEW to be working
#include <SparrowRenderer/sparrowrenderer.h>

#ifdef COMPATIBILITY_DIMITRI
#include <QOpenGLWidget>
#define OPENGL_WIDGET_NAME QOpenGLWidget
#else
#include <QGLWidget>
#define OPENGL_WIDGET_NAME QGLWidget
#endif

#include <QTimer>

#include "mycamera.h"
#include "scenemanager.h"

class ForwardModule;
class PickModule;

class DrawWidget : public OPENGL_WIDGET_NAME
{
    private:
        Q_OBJECT

        SparrowRenderer renderer;
        MyCamera camera;
        QTimer glRefreshTimer;
        SceneManager sceneManager;
        ForwardModule *forward;
        PickModule *pick;

        QPoint lastMousePos;
        glm::vec4 grabPos;
        bool grabbedRotateCamera;
        bool grabbedMoveCamera;
        bool grabbedRotateObject;
        bool grabbedMoveObject;
        bool paused;
        float timeRate;

        const FrameBuffer *qtFBO;

        void initPipeline();
        glm::vec3 getRandomPos();

    protected:
        // Output
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL(int w, int h);

        // Input
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

    public:
        DrawWidget(QWidget *parent = 0);

    public slots:
        void addMesh();
        void addParticles();
        void resetScene();
        void resetCamera() {camera.reset();}
        void setPaused(bool isPaused) {paused = isPaused;}
        void setTimeRate(int percent) {timeRate = percent/100.f;}
        void setNbIterations(int nb) {sceneManager.setNbIterations(nb);}

    private slots:
        void updateScene();

    signals:
        void updateFPS(double, double);
};

#endif // DRAWWIDGET_H
