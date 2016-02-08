#include "framebuffer.h"
#include "texture.h"
#include "glassert.h"

const FrameBuffer* FrameBuffer::screen = new FrameBuffer(0);

FrameBuffer::FrameBuffer()
{
    glAssert(glGenFramebuffers(1, &fbo));
}

FrameBuffer::~FrameBuffer()
{
    if(fbo != 0)
        glAssert(glDeleteFramebuffers(1, &fbo));
}

void FrameBuffer::addTexture(Texture* tex, GLenum attachment)
{
    if(fbo != 0)
    {
        textures.push_back(tex);
        bindFBO();
        if(tex->isCubeMap())
        {
            // TODO
            // http://cg.siomax.ru/index.php/computer-graphics/10-one-pass-rendering-to-cube-map
        }
        else
        {
            glAssert(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->getTarget(), tex->getId(), 0));
            if(attachment != GL_DEPTH_ATTACHMENT)
                attachments.push_back(attachment);
        }

    }
}

void FrameBuffer::initColorAttachments()
{
    if(fbo != 0 && attachments.size() != 0)
    {
        bindFBO();
        glAssert(glDrawBuffers(attachments.size(), attachments.data()));
    }
    else
        glAssert(glDrawBuffer(GL_NONE));
    check();
}

void FrameBuffer::deleteTextures()
{
    for(Texture* t : textures)
        delete(t);
}

void FrameBuffer::bindFBO(GLenum target) const
{
    glAssert(glBindFramebuffer(target, fbo));
}

bool FrameBuffer::check()
{
    GLenum err;
    glAssert(err = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (err != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FBO not complete (error = " << err << ") : ";
        switch (err) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        default:
            std::cerr << "Unknown ERROR";
        }
        std::cerr << std::endl;
    }
    return err == GL_FRAMEBUFFER_COMPLETE;
}

Texture* FrameBuffer::getTexture(int texId)
{
    if(fbo != 0)
        return textures[texId];
    else
        return NULL;
}

