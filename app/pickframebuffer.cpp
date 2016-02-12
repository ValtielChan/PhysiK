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
    Texture* tex = new Texture(GL_RGBA, GL_RGBA32F, w, h, GL_FLOAT, GL_TEXTURE_RECTANGLE);
    addTexture(tex, GL_COLOR_ATTACHMENT1);
    initColorAttachments();
}

int PickFramebuffer::getObjectId(int x, int y)
{
    bindFBO();
    Texture* tex = textures[0];
    tex->bind(0);
    glm::vec4 *val = new glm::vec4[w*h];
    glAssert(glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, GL_FLOAT, val));
    int id = int(val[x + y*w].w);
    delete[] val;
    return id;
}
