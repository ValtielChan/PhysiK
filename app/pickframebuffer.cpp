#include "pickframebuffer.h"

#include <SparrowRenderer/texture.h>
#include <SparrowRenderer/glassert.h>
#include <glm/ext.hpp>

PickFramebuffer::PickFramebuffer() :
    FrameBuffer(0)
{
    attachments.push_back(GL_COLOR_ATTACHMENT0);
}

void PickFramebuffer::resize(GLint qtFramebuffer, int width, int height)
{
    w = width;
    h = height;
    fbo = qtFramebuffer;
    for(Texture *t : textures)
    {
        attachments.pop_back();
        delete t;
    }
    textures.clear();
    Texture* tex = new Texture(GL_RGB, GL_RGB32F, w, h, GL_FLOAT, GL_TEXTURE_RECTANGLE);
    addTexture(tex, GL_COLOR_ATTACHMENT1);
    initColorAttachments();
}

glm::vec3 PickFramebuffer::getObjectId(int x, int y)
{
    bindFBO();
    Texture* tex = textures[0];
    tex->bind(0);
    glm::vec3 *val = new glm::vec3[w*h];
    glAssert(glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_RGB, GL_FLOAT, val));
    glm::vec3 ret = val[x + (h-y)*w];
    ret.z -= 1; // clearColor compensation
    delete[] val;
    return ret;
}
