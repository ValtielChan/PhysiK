#ifndef PICKFRAMEBUFFER_H
#define PICKFRAMEBUFFER_H

#include <SparrowRenderer/framebuffer.h>

class PickFramebuffer : public FrameBuffer
{
        int w, h;

    public:
        PickFramebuffer();

        void resize(GLint qtFramebuffer, int width, int height);

        int getObjectId(int x, int y);
};

#endif // PICKFRAMEBUFFER_H
