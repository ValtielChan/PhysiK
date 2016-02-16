#include "pickmodule.h"

#include <SparrowRenderer/framebuffer.h>
#include <SparrowRenderer/texture.h>
#include <SparrowRenderer/glassert.h>
#include <glm/ext.hpp>
#include <SparrowRenderer/shader.h>

const GLfloat PickModule::vertices[] = {
    -1.0f, -1.0f,
     3.0f, -1.0f,
    -1.0f,  3.0f
};

const std::string PickModule::vertSource =
    "#version 330 core\n\
    layout(location = 0)in vec2 inPosition;\n\
    void main(void) {\n\
        gl_Position = vec4(inPosition, 0.0, 1.0);\n\
    }\n";

const std::string PickModule::fragSource =
    "#version 330 core\n\
    uniform sampler2DRect colorSampler;\n\
    out vec4 outColor;\n\
    void main(void) {\n\
        outColor = texelFetch(colorSampler, ivec2(gl_FragCoord.xy));\n\
    }\n";

PickModule::PickModule(int width, int height) :
    fbo(NULL), rbo(0),
    renderTarget(FrameBuffer::screen)
{
    // create FBO
    resize(width, height);

    // create pass-through shader
    shader = new Shader(vertSource, fragSource);

    // create VAO
    glAssert(glGenVertexArrays(1, &vao));
    glAssert(glBindVertexArray(vao));
    glAssert(glGenBuffers(1, &vbo));
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    glAssert(glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), vertices, GL_STATIC_DRAW));
    glAssert(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, NULL));
    glAssert(glEnableVertexAttribArray(0));
}

PickModule::~PickModule()
{
    glAssert(glDeleteVertexArrays(1, &vao));
    glAssert(glDeleteBuffers(1, &vbo));
    fbo->deleteTextures();
    glAssert(glDeleteRenderbuffers(1, &rbo));
    delete fbo;
    delete shader;
}

void PickModule::renderGL(Camera* myCamera, Scene* scene)
{
    // draw on screen
    renderTarget->bindFBO();

    glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    glAssert(glDisable(GL_BLEND));
    glAssert(glDisable(GL_DEPTH_TEST));
    glAssert(glViewport(0, 0, w, h));
    glAssert(glClear(GL_COLOR_BUFFER_BIT));

    // bind shader and uniforms
    shader->bind();
    shader->bindInteger(shader->getLocation("colorSampler"), 0);
    fbo->getTexture(0)->bind(0);

    // draw geometry
    glAssert(glBindVertexArray(vao));
    glAssert(glDrawArrays(GL_TRIANGLES, 0, 3));

    glAssert(glEnable(GL_DEPTH_TEST));
}

void PickModule::resize(int width, int height)
{
    w = width;
    h = height;
    if(fbo != NULL)
    {
        fbo->deleteTextures();
        glAssert(glDeleteRenderbuffers(1, &rbo));
        delete(fbo);
    }
    fbo = new FrameBuffer();
    fbo->bindFBO();
    glAssert(glGenRenderbuffers(1, &rbo));
    glAssert(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    glAssert(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h));
    glAssert(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo));

    Texture *colorTex = new Texture(GL_RGBA, GL_RGBA32F, w, h, GL_FLOAT, GL_TEXTURE_RECTANGLE);
    fbo->addTexture(colorTex, GL_COLOR_ATTACHMENT0);
    Texture *pickTex = new Texture(GL_RGBA, GL_RGBA32F, w, h, GL_FLOAT, GL_TEXTURE_RECTANGLE);
    fbo->addTexture(pickTex, GL_COLOR_ATTACHMENT1);

    fbo->initColorAttachments();
}

glm::vec3 PickModule::getObjectInfo(int x, int y)
{
    fbo->bindFBO();
    fbo->getTexture(1)->bind(0);
    glm::vec4 *val = new glm::vec4[w*h];
    glAssert(glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, GL_FLOAT, val));
    glm::vec4 ret = val[x + (h-y)*w];
    ret.z -= 1; // clearColor compensation
    delete[] val;
    return glm::vec3(ret);
}
