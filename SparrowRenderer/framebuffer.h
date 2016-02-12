#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glew.h"
#include <vector>

class Texture;

class FrameBuffer
{
protected:
    FrameBuffer(int id) : allocated(false), fbo(id) {}
    bool check();
    const bool allocated;
    GLuint fbo;
    std::vector<Texture*> textures;
    std::vector<GLuint> attachments;

public:
    FrameBuffer();
    ~FrameBuffer();
    void addTexture(Texture* tex, GLenum attachment);
    void initColorAttachments();
    void deleteTextures();
    void bindFBO(GLenum target = GL_FRAMEBUFFER) const;
    Texture* getTexture(int texId);
    static const FrameBuffer* screen;
};

#endif // FRAMEBUFFER_H
