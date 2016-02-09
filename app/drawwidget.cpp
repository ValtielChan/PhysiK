#include "drawwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <SparrowRenderer/crappymodule.h>
#include <SparrowRenderer/forwardmodule.h>
#include "wavefrontmesh.h"
#include <QCoreApplication>
#include <QFileDialog>
#include "qtutils.h"

DrawWidget::DrawWidget(QWidget *parent) :
    QGLWidget(parent)
{
    renderer.setClearColor(glm::vec3(0.2f, 0.4f, 0.3f));
    renderer.setCamera(&camera);
    renderer.setScene(sceneManager.getScene());
    connect(&glRefreshTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    glRefreshTimer.start(25);
}

void DrawWidget::initializeGL()
{
    renderer.initGL(width(), height());
    resizeGL(width(), height());
    initPipeline();
    setFocus(); // enables keyboard events
}

void DrawWidget::paintGL()
{
    renderer.renderGL();
}

void DrawWidget::resizeGL(int w, int h)
{
    renderer.resizeGL(w, h);
    repaint();
}

void DrawWidget::initPipeline()
{
    if(renderer.isModernOpenGLAvailable())
    {
        forward = new ForwardModule();
        QString frag = QtUtils::fileToString(QCoreApplication::applicationDirPath().append("/../shaders/forward.frag.glsl"));
        QString vert = QtUtils::fileToString(QCoreApplication::applicationDirPath().append("/../shaders/forward.vert.glsl"));
        ShaderSource *source = new ShaderSource();
        source->setSource(frag.toStdString().c_str(), ShaderSource::FRAGMENT);
        source->setSource(vert.toStdString().c_str(), ShaderSource::VERTEX);
        forward->setShaderSource(source);
        forward->compileShaders(sceneManager.getScene());
        renderer.addModule(forward, "forward");
    }
    else
        renderer.addModule(new CrappyModule(), "crappy");
}

void DrawWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_A :
        {
            int n = 5;
            std::vector<glm::vec3> particles;
            for(int i=0; i<n; ++i)
                particles.push_back(glm::vec3(rand()%100, rand()%100, rand()%100)/10.f);
            sceneManager.addParticleGroup(particles);
            forward->compileShaders(sceneManager.getScene()); // dynamically recompiling shaders from materials in the scene
            printf("added %d particles\n", n);
        }
        case Qt::Key_Z :
        {
            int n = 1;
            std::vector<glm::vec3> particles;
            for(int i=0; i<n; ++i)
                particles.push_back(glm::vec3(rand()%100, rand()%100, rand()%100)/10.f);
            sceneManager.addParticleGroup(particles);
            forward->compileShaders(sceneManager.getScene()); // dynamically recompiling shaders from materials in the scene
            printf("added %d particles\n", n);
        }
        break;
        case Qt::Key_E :
        {
            WavefrontMesh meshLoader;
            QString filename = QFileDialog::getOpenFileName(this, tr("Open Wavefront object"),
                                                            QCoreApplication::applicationDirPath().append("/.."),
                                                            tr("Wavefront Object (*.obj)"));
            std::vector<Mesh*> meshes = meshLoader.loadMesh(filename);
            for(Mesh *m : meshes)
            {
                GeometryNode* node = new GeometryNode();
                node->mesh = m;
                m->initGL();
                sceneManager.addNode(node);
            }
            forward->compileShaders(sceneManager.getScene());
        }
        break;
        case Qt::Key_R :
            camera.reset();
            printf("reseted camera positionning\n");
        break;
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(grabbed)
        camera.mouseMove(event->globalX() - lastMousePos.x(), event->globalY() - lastMousePos.y());
    lastMousePos = event->globalPos();
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    grabbed = true;
    lastMousePos = event->globalPos();
}
void DrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    grabbed = false;
}
