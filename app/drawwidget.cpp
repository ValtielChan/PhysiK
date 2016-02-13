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
    paused(false)
{
    fbo = new PickFramebuffer();
    renderer.setClearColor(glm::vec3(0.1804f, 0.1647f, 0.1490f)*0.5f);
    renderer.setCamera(&camera);
    renderer.setScene(sceneManager.getScene());
    connect(&glRefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    glRefreshTimer.start(25);
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
            particles.push_back(getRandomPos()*10.f + glm::vec3(0, 6, 0));
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
    if(!paused)
        sceneManager.update(0.025f);
    repaint();
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
            addMesh();
        break;
        case Qt::Key_R :
            resetScene();
        break;
        case Qt::Key_T :
            camera.reset();
        break;
        case Qt::Key_Space :
            paused = !paused;
        break;
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(grabbedLeft)
        camera.rotateCamera(event->globalX() - lastMousePos.x(), event->globalY() - lastMousePos.y());
    if(grabbedRight)
        camera.moveCamera(event->globalX() - lastMousePos.x(), event->globalY() - lastMousePos.y());
    lastMousePos = event->globalPos();
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::LeftButton :
            grabbedLeft = true;
            lastMousePos = event->globalPos();
            break;
        case Qt::RightButton :
            grabbedRight = true;
            lastMousePos = event->globalPos();
            break;
        case Qt::MiddleButton :
        if(renderer.isModernOpenGLAvailable())
        {
            glm::vec3 info = fbo->getObjectId(event->x(), event->y());
            int id = int(info.z);
            glm::vec4 pos(float(event->x())*2/width() - 1.f, float(height() - event->y())*2/height() - 1.f, info.x, 1);
            pos /= info.y;
            // le contenu de pos semble correctement initialisé (dans le frustum)

            glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();

            glm::vec4 clickPos = glm::inverse(mvp) * pos;

            /*SceneIterator<GeometryNode*> *plop = sceneManager.getScene()->getGeometry();
            plop->next();
            glm::vec3 particle1 = plop->getItem()->mesh->instances_offsets[0];

            glm::vec4 theoriticalPos = mvp * glm::vec4(particle1.x, particle1.y, particle1.z, 1);
            theoriticalPos /= theoriticalPos.w;

            printf("plop\n");*/
            if(id > 0)
                camera.setCenter(glm::vec3(clickPos.x, clickPos.y, clickPos.z));
        }
            break;
        default:
            break;
    }
}
void DrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    grabbedLeft = false;
}

void DrawWidget::wheelEvent(QWheelEvent *event)
{
    camera.mouseScroll(event->delta());
}
