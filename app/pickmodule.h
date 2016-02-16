#ifndef PICKFRAMEBUFFER_H
#define PICKFRAMEBUFFER_H

#include <string>
#include <SparrowRenderer/glew.h>
#include <SparrowRenderer/module.h>
#include <glm/vec3.hpp>

class FrameBuffer;
class Shader;

class PickModule : public Module
{
    private:
        int w, h;
        FrameBuffer *fbo;
        Shader *shader;
        GLuint vao;
        GLuint vbo;
        GLuint rbo;
        const FrameBuffer *renderTarget;

        static const GLfloat vertices[];
        static const std::string vertSource;
        static const std::string fragSource;

    public:
        PickModule(int width, int height);
        ~PickModule();

        virtual void renderGL(Camera* myCamera, Scene* scene);
        virtual void resize(int width, int height);

        FrameBuffer* getFrameBuffer() {return fbo;}

        void setRenderTarget(const FrameBuffer *target) {renderTarget = target;}

        glm::vec3 getObjectInfo(int x, int y);
};

#endif // PICKFRAMEBUFFER_H
