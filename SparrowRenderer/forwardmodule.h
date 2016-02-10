#ifndef FORWARDMODULE_H
#define FORWARDMODULE_H

#include "glew.h"
#include "module.h"
#include <vector>
#include <cstddef>
#include "shadersource.h"
#include "material.h"
#include "framebuffer.h"

class Light;
class Scene;
class PhongEntity;

class ForwardModule : public Module
{
public:
    ForwardModule() :
        shaderSources(NULL),
        renderTarget(FrameBuffer::screen),
        isWireframe(false)
    {}

    void renderGL(Camera* myCamera, Scene* scene);
    bool requiresModernOpenGL() {return true;} // write some compatibility code to change that to false

    // modern opengl methods

    void setShaderSource(ShaderSource* source);
    void compileShaders(Scene* scene);

    void setRenderTarget(const FrameBuffer* target);
    void setWireframe(bool wireframe) {isWireframe = wireframe;}

private:
    static const char* const flagStr[NB_FLAGS];

    ShaderSource* shaderSources;
    std::vector<Shader*> shaders;
    std::vector<unsigned int> geometryFlagList;
    std::vector<unsigned int> lightFlagList;
    const FrameBuffer* renderTarget;

    bool isWireframe;

    void lightPass(Camera* myCamera, Scene* scene, Light* light);
};

#endif // FORWARDMODULE_H
