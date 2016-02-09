#include "glew.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstdio>
#include "sparrowrenderer.h"
#include "glassert.h"
#include "camera.h"
#include "framebuffer.h"
#include "module.h"
#include <chrono>
#include <string.h>
// main methods

bool SparrowRenderer::modernOpenglAvailable = false;

void SparrowRenderer::initGL(int w, int h, bool forceCrappy)
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        fprintf(stderr, "Warning: glewInit failed!\n");
    if (!forceCrappy &&
        GLEW_ARB_vertex_program   &&
        GLEW_ARB_fragment_program &&
        GLEW_ARB_texture_float    &&
        GLEW_ARB_draw_buffers     &&
        GLEW_ARB_framebuffer_object &&
        glewIsSupported("GL_VERSION_3_3") &&
        GLEW_VERSION_3_3 &&
        strcmp("3.30",(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION))<=0)
    {
        modernOpenglAvailable = true;
        printf("Modern OpenGL available.\n");
    }
    else
    {
        modernOpenglAvailable = false;
        fprintf(stderr, "Warning: modern OpenGL not supported!\nEnabling fallback crappy rendering mode\n");
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor " << glGetString(GL_VENDOR) << std::endl;

    glAssert(glEnable(GL_DEPTH_TEST));
    glAssert(glEnable(GL_CULL_FACE));
    glAssert(glEnable(GL_TEXTURE_2D));
    resizeGL(w, h);
}

void SparrowRenderer::destroyGL()
{
    for(ModuleNode &m : modules)
        delete m.module;
    modules.clear();
}

void SparrowRenderer::resizeGL(int w, int h)
{
    width = w;
    height = h;
    glAssert(glViewport(0, 0, width, height));
    if(camera != NULL)
        camera->resize(width, height);
    for(ModuleNode &mn : modules)
        mn.module->resize(w, h);
}

void SparrowRenderer::renderGL()
{
    FrameBuffer::screen->bindFBO();
    glAssert(glViewport(0, 0, width, height));
    glAssert(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1));
    glAssert(glClearDepth(1.0));
    glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    for(ModuleNode &m : modules)
    {
        if(m.isEnabled)
        {
            std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
            m.module->renderGL(getCamera(), scene);
            m.seconds = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - t).count();
        }
    }
}

bool SparrowRenderer::isModernOpenGLAvailable()
{
    return modernOpenglAvailable;
}

// clock methods

double SparrowRenderer::getFPS()
{
    double totalTime = 0;
    for(ModuleNode &m : modules)
    {
        if(m.isEnabled)
            totalTime += m.seconds;
    }
    return 1./totalTime;
}

double SparrowRenderer::getModuleTime(int moduleId)
{
    return modules[moduleId].seconds;
}

// module methods

void SparrowRenderer::addModule(Module* myModule, std::string name)
{
    if(modernOpenglAvailable || !myModule->requiresModernOpenGL())
        modules.push_back(ModuleNode(myModule, name));
}

int SparrowRenderer::getNbModules()
{
    return modules.size();
}

void SparrowRenderer::setModuleEnabled(std::string module, bool isEnabled)
{
    for(ModuleNode &node : modules)
    {
        if(node.name.compare(module) == 0)
            node.isEnabled = isEnabled;
    }
}

// camera methods

void SparrowRenderer::setCamera(Camera* myCamera)
{
    camera = myCamera;
}

Camera* SparrowRenderer::getCamera()
{
    return camera;
}

// scene methods

void SparrowRenderer::setScene(Scene* myScene)
{
    scene = myScene;
}

Scene* SparrowRenderer::getScene()
{
    return scene;
}


