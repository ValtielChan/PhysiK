#ifndef PICKFRAMEBUFFER_H
#define PICKFRAMEBUFFER_H

#include <SparrowRenderer/framebuffer.h>
#include <glm/vec3.hpp>

class PickFramebuffer : public FrameBuffer
{
        int w, h;

    public:
        PickFramebuffer();

        void resize(GLint qtFramebuffer, int width, int height);

        glm::vec3 getObjectId(int x, int y);
};

#endif // PICKFRAMEBUFFER_H
