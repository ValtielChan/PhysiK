#include "drawwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <SparrowRenderer/crappymodule.h>
#include <SparrowRenderer/forwardmodule.h>
#include <SparrowRenderer/glassert.h>
#include "wavefrontmesh.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QInputDialog>
#include "particledialog.h"
#include "pickframebuffer.h"
#include "qtutils.h"
#include <ctime>
#include <glm/ext.hpp>

DrawWidget::DrawWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    paused(false),slowmotion(false)
{
    fbo = new PickFramebuffer();
    renderer.setClearColor(glm::vec3(0.1804f, 0.1647f, 0.1490f)*0.5f);
    renderer.setCamera(&camera);
    renderer.setScene(sceneManager.getScene());
    connect(&glRefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    glRefreshTimer.start(16);
    std::srand(std::time(NULL));
}

void DrawWidget::initializeGL()
{
    renderer.initGL(width(), height(), false);
    resizeGL(width(), height());
    initPipeline();
    resetScene();
    setFocus(); // enables keyboard events
}

void DrawWidget::paintGL()
{
    renderer.renderGL();
}

void DrawWidget::resizeGL(int w, int h)
{
    if(renderer.isModernOpenGLAvailable())
    {
        GLint qtFramebuffer;
        glAssert(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &qtFramebuffer));
        fbo->resize(qtFramebuffer, width(), height());
    }
    renderer.resizeGL(w, h);
    repaint();
}

void DrawWidget::initPipeline()
{
    if(renderer.isModernOpenGLAvailable())
    {
        GLint qtFramebuffer;
        glAssert(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &qtFramebuffer));
        fbo->resize(qtFramebuffer, width(), height());

        forward = new ForwardModule();
        QString frag = QtUtils::fileToString(":shaders/shaders/forward.frag.glsl");
        QString vert = QtUtils::fileToString(":shaders/shaders/forward.vert.glsl");
        ShaderSource *source = new ShaderSource();
        source->setSource(frag.toStdString().c_str(), ShaderSource::FRAGMENT);
        source->setSource(vert.toStdString().c_str(), ShaderSource::VERTEX);
        forward->setShaderSource(source);
        forward->compileShaders(sceneManager.getScene());
        renderer.addModule(forward, "forward");
        forward->setRenderTarget(fbo);
    }
    else
        renderer.addModule(new CrappyModule(), "crappy");
}

glm::vec3 DrawWidget::getRandomPos()
{
    return glm::vec3(rand()%1000, rand()%1000, rand()%1000)/1000.f - 0.5f;
}

void DrawWidget::addMesh()
{
    WavefrontMesh meshLoader;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Wavefront object"),
                                                    QCoreApplication::applicationDirPath().append("/../assets"),
                                                    tr("Wavefront Object (*.obj)"));
    std::vector<Mesh*> meshes = meshLoader.loadMesh(filename);
    for(Mesh *m : meshes)
    {
        GeometryNode* node = new GeometryNode();
        node->mesh = m;
        m->initGL();
        sceneManager.addNode(node);
    }
    if(renderer.isModernOpenGLAvailable())
        forward->compileShaders(sceneManager.getScene());
}

void DrawWidget::addParticles()
{
    bool ok;
    static ParticleDialog *dialog = NULL;
    if(dialog == NULL)
        dialog = new ParticleDialog(this);
    ok = (dialog->exec() == QDialog::Accepted);
    if(ok)
    {
        ParticleProperties properties = dialog->getParticleProperties();

        std::vector<glm::vec3> particles;
        for(int i=0; i<properties.amount; ++i)
            particles.push_back(getRandomPos()*10.f + glm::vec3(0, 14, 0));
        sceneManager.addParticleGroup(properties, particles.data());

        if(renderer.isModernOpenGLAvailable())
            forward->compileShaders(sceneManager.getScene());
    }
}

void DrawWidget::resetScene()
{
    sceneManager.resetScene();
    if(renderer.isModernOpenGLAvailable())
        forward->compileShaders(sceneManager.getScene());
}

void DrawWidget::update()
{
    double physicsTime = 0;
    if(!paused)
        physicsTime = sceneManager.update(slowmotion?0.002f:0.025f);
    camera.update();
    repaint();
    emit updateFPS(physicsTime, 1./renderer.getFPS());
}

void DrawWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_A :
            addParticles();
        break;
        case Qt::Key_Z :
            addMesh();
        break;
        case Qt::Key_E :
            resetScene();
        break;
        case Qt::Key_R :
            resetCamera();
        break;
        case Qt::Key_Space :
            emit pauseEvent();
        break;
        case Qt::Key_S:
            slowmotion = !slowmotion;
        break;
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(grabbedRotateCamera)
        camera.rotateCamera(event->globalX() - lastMousePos.x(), event->globalY() - lastMousePos.y());
    if(grabbedMoveCamera)
    {
        float x = float(event->x())*2/width() - 1.f;
        float y = float(height() - event->y())*2/height() - 1.f;
        glm::vec4 pos(x*grabPos.w, y*grabPos.w, grabPos.z, grabPos.w);
        glm::mat4 inverseMVP = glm::inverse(camera.getProjectionMatrix() * camera.getViewMatrix());
        glm::vec4 diff = (inverseMVP * grabPos) - (inverseMVP * pos);
        grabPos = pos;
        camera.moveCenter(glm::vec3(diff.x, diff.y, diff.z));
    }
    lastMousePos = event->globalPos();
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    int id = 0;
    glm::vec3 pxInfo;
    if(renderer.isModernOpenGLAvailable())
    {
        pxInfo = fbo->getObjectId(event->x(), event->y());
        id = int(pxInfo.z);
    }
    if(id == 0)
        pxInfo = camera.getDefaultPxInfo();
    // opengl frustum has boundaries of [-1, 1]
    glm::vec4 pos(float(event->x())*2/width() - 1.f, float(height() - event->y())*2/height() - 1.f, pxInfo.x*2 - 1, 1);
    pos /= pxInfo.y;
    grabPos = pos;
    switch(event->button())
    {
        case Qt::LeftButton :
            if(event->modifiers() & Qt::ControlModifier)
                grabbedRotateObject = true;
            else
                grabbedRotateCamera = true;
            break;
        case Qt::RightButton :
            if(event->modifiers() & Qt::ControlModifier)
                grabbedMoveObject = true;
            else
                grabbedMoveCamera = true;
            break;
        default:
            break;
    }
    lastMousePos = event->globalPos();
}

void DrawWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(renderer.isModernOpenGLAvailable())
        {
            glm::vec3 info = fbo->getObjectId(event->x(), event->y());
            int id = int(info.z);
            if(id > 0)
            {
                // opengl frustum has boundaries of [-1, 1]
                glm::vec4 pos(float(event->x())*2/width() - 1.f, float(height() - event->y())*2/height() - 1.f, info.x*2 - 1, 1);
                pos /= info.y;
                glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();
                // applying inverse mvp
                glm::vec4 clickPos = glm::inverse(mvp) * pos;
                camera.setTarget(glm::vec3(clickPos.x, clickPos.y, clickPos.z));
            }
        }
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::LeftButton :
            grabbedRotateCamera = false;
            grabbedRotateObject = false;
            break;
        case Qt::RightButton :
            grabbedMoveCamera = false;
            grabbedMoveObject = false;
            break;
        default:
            break;
    }
}

void DrawWidget::wheelEvent(QWheelEvent *event)
{
    camera.mouseScroll(event->delta());
}
